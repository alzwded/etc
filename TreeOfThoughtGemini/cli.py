#!/usr/bin/env python3
import subprocess
import json
import threading
import os
import sys
import re
import readline
from dataclasses import dataclass
from typing import List, Optional, Dict, Any

# --- Data Structures for Meta-Reasoning ---

@dataclass
class Opinion:
    id: int
    text: str

@dataclass
class Discovery:
    opinion: Opinion
    report: str

@dataclass
class Plan:
    opinion: Opinion
    discovery: Discovery
    text: str
    score: float = 0.0

# --- Asynchronous Linux Terminal Manager ---

class TerminalManager:
    """Handles the lifecycle and buffering of local Linux subprocesses per the ACP terminal/ schemas."""
    def __init__(self):
        self.terminals = {}
        self.lock = threading.Lock()
        self.next_id = 1

    def create(self, command: str, args: List[str], cwd: str, env) -> str:
        term_id = f"term_{self.next_id}"
        self.next_id += 1
        
        cmd = [command] + (args or [])
        run_env = os.environ.copy()
        if env:
            for e in env:
                run_env[e['name']] = e['value']
        
        # Initiate subprocess targeted for Linux shells
        proc = subprocess.Popen(
            cmd, cwd=cwd, env=run_env,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            text=True, bufsize=1
        )
        
        self.terminals[term_id] = {
            "process": proc,
            "output_buffer": [],
            "lock": threading.Lock()
        }
        
        # Background daemon to continually flush and store output
        def read_output():
            for line in iter(proc.stdout.readline, ''):
                with self.terminals[term_id]["lock"]:
                    self.terminals[term_id]["output_buffer"].append(line)
            proc.stdout.close()
            
        threading.Thread(target=read_output, daemon=True).start()
        return term_id

    def get_output(self, term_id: str) -> tuple:
        if term_id not in self.terminals:
            raise ValueError(f"Unknown terminal ID: {term_id}")
            
        term = self.terminals[term_id]
        with term["lock"]:
            out = "".join(term["output_buffer"])
            term["output_buffer"].clear()
        
        poll = term["process"].poll()
        exit_status = {"code": poll} if poll is not None else None
        return out, exit_status

    def wait_for_exit(self, term_id: str) -> Dict[str, int]:
        if term_id not in self.terminals:
            raise ValueError(f"Unknown terminal ID: {term_id}")
            
        term = self.terminals[term_id]
        term["process"].wait()
        return {"code": term["process"].returncode}

    def kill(self, term_id: str):
        if term_id in self.terminals:
            self.terminals[term_id]["process"].terminate()

    def release(self, term_id: str):
        if term_id in self.terminals:
            self.kill(term_id)
            del self.terminals[term_id]

# --- Standardized ACP Client over JSON-RPC 2.0 (Stdio) ---

class ACPClientError(Exception):
    """Exception raised for structural errors within the Agent Client Protocol."""
    pass

class ACPClient:
    """
    A highly concurrent client establishing an Agent Client Protocol (ACP) connection 
    to the Gemini CLI via JSON-RPC 2.0 over standard input/output.
    """
    def __init__(self, cli_args: List[str] = None):
        if cli_args is None:
            # Enable ACP mode natively without proprietary wrappers
            cli_args = ["gemini", "--acp", "--yolo"]
            
        self.request_id_counter = 0
        self.pending_requests: Dict = {}
        self.session_buffers: Dict[str, List[str]] = {}
        
        self.term_manager = TerminalManager()
        self.lock = threading.Lock()
        
        try:
            # Spawn the agent as a subprocess with line buffering
            self.process = subprocess.Popen(
                cli_args,
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.DEVNULL, # Isolate stderr to prevent protocol pollution
                text=True,
                bufsize=1 
            )
        except FileNotFoundError:
            raise ACPClientError("Failed to execute Gemini CLI. Ensure 'gemini' is in the system PATH.")

        # Instantiate the daemon reader thread for asynchronous notification processing
        self.reader_thread = threading.Thread(target=self._read_stdout_loop, daemon=True)
        self.reader_thread.start()

        # Execute Mandatory Protocol Handshake Lifecycle
        self._initialize_protocol()
        self._authenticate_protocol()

    def _get_next_id(self) -> int:
        """Generates a monotonically increasing JSON-RPC request identifier."""
        with self.lock:
            self.request_id_counter += 1
            return self.request_id_counter

    def _send_request(self, method: str, params: Optional = None) -> Dict:
        """
        Constructs and transmits a JSON-RPC 2.0 request over the subprocess standard input,
        blocking the main thread until the correlating response is parsed by the reader thread.
        """
        req_id = self._get_next_id()
        payload = {
            "jsonrpc": "2.0",
            "id": req_id,
            "method": method
        }
        if params is not None:
            payload["params"] = params

        # Establish localized synchronization event for this specific request
        event = threading.Event()
        with self.lock:
            self.pending_requests[req_id] = {
                "event": event,
                "response": None
            }

        # Serialize to newline-delimited JSON and transmit
        raw_json = json.dumps(payload)
        try:
            self.process.stdin.write(raw_json + "\n")
            self.process.stdin.flush()
        except BrokenPipeError:
            raise ACPClientError("The ACP subprocess terminated unexpectedly.")

        # Suspend thread execution until the reader thread resolves the correlation event
        event.wait(timeout=300.0)

        with self.lock:
            result_data = self.pending_requests.pop(req_id, {}).get("response")
            
        if result_data is None:
            raise ACPClientError(f"Request {req_id} timed out without a response.")
            
        if "error" in result_data:
            err = result_data["error"]
            raise ACPClientError(f"JSON-RPC Error [{err.get('code')}]: {err.get('message')}")
            
        return result_data.get("result", {})

    def _read_stdout_loop(self):
        """
        Daemon loop intercepting standard output. Routes incoming notifications to session buffers,
        resolves blocked requests, and actively processes Agent-to-Client method calls.
        """
        for line in self.process.stdout:
            line = line.strip()
            if not line:
                continue
                
            try:
                msg = json.loads(line)
            except json.JSONDecodeError:
                continue # Discard unparseable stdout data safely

            # Classify payload variant
            if "id" in msg and ("result" in msg or "error" in msg):
                # JSON-RPC Response variant: Matches a pending Client-to-Agent request
                req_id = msg["id"]
                with self.lock:
                    if req_id in self.pending_requests:
                        self.pending_requests[req_id]["response"] = msg
                        self.pending_requests[req_id]["event"].set()
                        
            elif "id" in msg and "method" in msg:
                # Agent-to-Client Request Variant (e.g., terminal/create, fs/read_text_file)
                req_id = msg["id"]
                method_name = msg["method"]
                params = msg.get("params", {})
                
                response_payload = {"jsonrpc": "2.0", "id": req_id}
                
                try:
                    # Linux YOLO Mode Execution Handlers
                    if method_name == "terminal/create":
                        term_id = self.term_manager.create(
                            params.get("command"),
                            params.get("args"),
                            params.get("cwd", os.getcwd()),
                            params.get("env")
                        )
                        response_payload["result"] = {"terminalId": term_id}
                        
                    elif method_name == "terminal/output":
                        out, status = self.term_manager.get_output(params.get("terminalId"))
                        res = {"output": out}
                        if status:
                            res = status
                        response_payload["result"] = res
                        
                    elif method_name == "terminal/wait_for_exit":
                        status = self.term_manager.wait_for_exit(params.get("terminalId"))
                        response_payload["result"] = {"exitStatus": status}
                        
                    elif method_name == "terminal/kill":
                        self.term_manager.kill(params.get("terminalId"))
                        response_payload["result"] = {}
                        
                    elif method_name == "terminal/release":
                        self.term_manager.release(params.get("terminalId"))
                        response_payload["result"] = {}
                        
                    elif method_name == "fs/read_text_file":
                        file_path = params.get("path")
                        with open(file_path, "r", encoding="utf-8") as f:
                            response_payload["result"] = {"text": f.read()}
                            
                    elif method_name == "fs/write_text_file":
                        file_path = params.get("path")
                        with open(file_path, "w", encoding="utf-8") as f:
                            f.write(params.get("text", ""))
                        response_payload["result"] = {}
                        
                    else:
                        response_payload["error"] = {
                            "code": -32601,
                            "message": f"Method '{method_name}' not implemented."
                        }
                except Exception as e:
                    response_payload["error"] = {
                        "code": -32603,
                        "message": str(e)
                    }

                # Transmit execution results back to the agent
                try:
                    self.process.stdin.write(json.dumps(response_payload) + "\n")
                    self.process.stdin.flush()
                except BrokenPipeError:
                    pass
            
            elif "method" in msg and msg["method"] == "session/update":
                # JSON-RPC Notification variant: Streaming payload intercept
                params = msg.get("params", {})
                session_id = params.get("sessionId")
                update_data = params.get("update", {})
                
                # Intercept exact text chunks based on protocol schema requirements
                if update_data.get("sessionUpdate") == "agent_message_chunk":
                    content = update_data.get("content", {})
                    if content.get("type") == "text" and "text" in content:
                        chunk_text = content["text"]
                        with self.lock:
                            if session_id not in self.session_buffers:
                                self.session_buffers[session_id] = [chunk_text]
                            else:
                                self.session_buffers[session_id].append(chunk_text)

    def _initialize_protocol(self):
        """
        Executes the mandatory ACP initialization sequence, establishing capability bounds.
        Enables 'YOLO mode' by granting full terminal and file system access.
        """
        init_params = {
            "protocolVersion": 1,
            "clientCapabilities": {
                "fs": {
                    "readTextFile": True,
                    "writeTextFile": True
                },
                "terminal": True
            },
            "clientInfo": {
                "name": "python-acp-harness-linux-yolo",
                "version": "1.0.0"
            }
        }
        self._send_request("initialize", init_params)

    def _authenticate_protocol(self):
        """
        Executes the optional Authentication handshake (Phase 2).
        Transmits required environment credentials into the protocol stream.
        """
        #auth_params = {
        #    "methodId": "gemini-api-key",
        #    "authMethod": {
        #        "id": "gemini-api-key",
        #        "name": "Use Gemini API key",
        #        "description": "Authenticates via the GEMINI_API_KEY environment variable"
        #    }
        #}
        auth_params = {
            "methodId": "oauth-personal",
            "authMethod": {
                "id": "oauth-personal",
                "name": "Sign in with google account",
                "description": "Authenticate with google account"
            }
        }
        try:
            self._send_request("authenticate", auth_params)
        except ACPClientError as e:
            # Safely ignore if the agent is already authenticated out-of-band or doesn't require it
            pass

    def create_session(self) -> str:
        """
        Instantiates a new stateful conversational session, binding it to the current directory.
        """
        cwd = os.path.abspath(os.getcwd())
        params = {
            "cwd": cwd,
            "mcpServers": []
        }
        result = self._send_request("session/new", params)
        return result.get("sessionId", "")

    def prompt(self, session_id: str, user_text: str) -> str:
        """
        Transmits a user prompt, manages the streaming aggregation, and extracts the finalized output.
        Complies strictly with the explicit ContentBlock schema array.
        """
        # Ensure session buffer is primed
        with self.lock:
            self.session_buffers[session_id] = []

        # Construct compliant prompt array utilizing explicit type definitions
        params = {
            "sessionId": session_id,
            "prompt": [
                {
                    "type": "text",
                    "text": user_text
                }
            ]
        }
        
        # Initiate the prompt turn
        response = self._send_request("session/prompt", params)
        
        # Verify normal completion termination
        if response.get("stopReason") == "error":
            return "[!] Agent encountered an internal error during generative processing."
            
        # Reconstruct monolithic string from streamed event buffers
        with self.lock:
            chunks = self.session_buffers.get(session_id,)
            final_text = "".join(chunks)
            # Purge buffer for subsequent turns
            self.session_buffers[session_id] = []
            
        return final_text

    def shutdown(self):
        """Gracefully terminates the subprocess link to prevent memory leaks."""
        if self.process:
            self.process.terminate()

# --- Analytical Processing Extraction Functions ---

def extract_json_array(text: str) -> List[str]:
    """Identifies and decodes a JSON array structurally embedded within conversational text."""
    match = re.search(r'\[.*?\]', text, re.DOTALL)
    if match:
        try:
            return json.loads(match.group(0))
        except json.JSONDecodeError:
            pass
    return

def extract_score(text: str) -> float:
    """Isolates standardized numerical viability scores from textual evaluations."""
    matches = re.findall(r"0\.\d+|1\.0|0|1", text)
    if matches:
        return float(matches[0])
    return 0.0

# --- Standardized Tree of Thoughts (ToT) Sub-Routine ---

def execute_tree_of_thoughts(acp: ACPClient, user_prompt: str, main_session_id: str) -> None:
    """
    Executes the 7-step ToT workflow. Ephemeral contexts are created sequentially
    to protect the primary thread from intermediate meta-reasoning payload pollution.
    """
    print("\n🌲 Initiating Tree of Thoughts reasoning over ACP...")

    # Step 1: Instantiate Ephemeral Context Alpha
    temp_session = acp.create_session()
    
    # Step 2: Generate exactly 3 distinct opinions
    print("🌲 Generating 3 structural interpretations...")
    prompt_step2 = (
        f"The user wants to execute the following task:\n'{user_prompt}'\n\n"
        "Generate exactly 3 distinct opinions/interpretations on how to approach this. "
        "Return your answer ONLY as a valid JSON array of 3 strings."
    )
    resp_text = acp.prompt(temp_session, prompt_step2)
    opinions_data = extract_json_array(resp_text)
    
    if not opinions_data:
        print("[!] ToT Aborted: Failed to generate structured JSON interpretations.")
        return
        
    opinions = [Opinion(id=i, text=op) for i, op in enumerate(opinions_data[:3])]
    for op in opinions:
        print(f"    - Interpretation {op.id + 1}: {op.text}")

    # Step 3: Explore the directory for contextual variables per opinion
    discoveries = []
    print("\n🌲 Exploring the environment for contextual constraints...")
    for op in opinions:
        # Utilize completely isolated context per interpretation fork
        fork_session = acp.create_session()
        prompt_step3 = (
            f"User Task: '{user_prompt}'\n"
            f"Interpretation: '{op.text}'\n\n"
            "Use your built-in tools to explore the current directory for info relevant to this. "
            "Report discoveries structurally. Do NOT execute the final task."
        )
        resp_text = acp.prompt(fork_session, prompt_step3)
        discoveries.append(Discovery(opinion=op, report=resp_text))

    # Step 4: Elaborate implementation plans based on discoveries
    plans: List[Plan] = []
    print("🌲 Drafting implementation plans based on contextual limits...")
    for disc in discoveries:
        plan_session = acp.create_session()
        prompt_step4 = (
            f"User Task: '{user_prompt}'\n"
            f"Interpretation: '{disc.opinion.text}'\n"
            f"Discoveries:\n{disc.report}\n\n"
            "Based on this, draft 3 distinct implementation plans. "
            "Return ONLY a valid JSON array of 3 strings."
        )
        resp_text = acp.prompt(plan_session, prompt_step4)
        plans_data = extract_json_array(resp_text)
        for p_text in plans_data[:3]:
            plans.append(Plan(opinion=disc.opinion, discovery=disc, text=p_text))

    # Step 5: Assign Viability Scores via secondary assessment session
    print("🌲 Scoring plans against execution viability matrices...")
    for plan in plans:
        score_session = acp.create_session()
        prompt_step5 = (
            f"Evaluate this plan.\n"
            f"Interpretation: '{plan.opinion.text}'\n"
            f"Plan: '{plan.text}'\n\n"
            "Score relevance and viability strictly from 0.0 to 1.0. "
            "Output ONLY the numeric score."
        )
        resp_text = acp.prompt(score_session, prompt_step5)
        plan.score = extract_score(resp_text)

    # Step 6: Optimal Path Selection Strategy
    if not plans:
        print("🌲 Failed to construct verifiable execution plans.")
        return
        
    best_plan = max(plans, key=lambda p: p.score)
    print(f"\n=============================================")
    print(f"🏅 OPTIMAL PLAN SELECTED (Score: {best_plan.score})")
    print(f"Plan: {best_plan.text}")
    print(f"=============================================\n")

    # Step 7: Inject Execution into the Persistent Primary Session Thread
    print("🌲 Injecting execution trajectory into primary session context...\n")
    prompt_step7 = (
        f"Regarding my prior task requirement: '{user_prompt}'\n"
        f"Internal logic has verified the following execution architecture. Execute this immediately:\n"
        f"'{best_plan.text}'\n\n"
        "Execute this step-by-step using available structural tools. Report back when finalized."
    )
    
    # Final transmission utilizes the persistence of main_session_id
    final_text = acp.prompt(main_session_id, prompt_step7)
    print(f"🤖 Agent:\n{final_text}")

# --- Main Command-Line Interface Event Loop ---

def main():
    print("Initializing Agent Client Protocol (ACP) Subprocess Link...")
    try:
        acp = ACPClient()
    except Exception as e:
        print(f"Failed to initialize robust protocol connection: {e}")
        sys.exit(1)
        
    # Establish persistent base context for conversational tracking
    primary_session_id = acp.create_session()

    print("==================================================")
    print("         Gemini ACP Chat & ToT Harness            ")
    print("==================================================")
    print(" - Input standard strings to modify primary conversational context.")
    print(" - Input '/tree <prompt>' to execute parallel logic workflows.")
    print(" - Input 'exit' to unbind processes.")
    print("==================================================\n")

    try:
        while True:
            user_input = input("👤 You: ").strip()
            
            if not user_input:
                continue
                
            if user_input.lower() in ['exit', 'quit']:
                break
                
            # Intercept advanced commands mapping to slash command specifications
            if user_input.startswith('/tree ') or user_input.startswith('/tot '):
                parts = user_input.split(' ', 1)
                if len(parts) > 1 and parts[1].strip():
                    execute_tree_of_thoughts(acp, parts[1].strip(), primary_session_id)
                else:
                    print("[!] Command format invalid. Valid structure: '/tree refactor module X'")
            else:
                # Execute standard textual transmission sequence
                resp_text = acp.prompt(primary_session_id, user_input)
                print(f"\n🤖 Agent:\n{resp_text}\n")
                
    except KeyboardInterrupt:
        pass
    finally:
        print("\nSevering process linkage to prevent memory leakage. Goodbye.")
        acp.shutdown()

if __name__ == "__main__":
    main()
