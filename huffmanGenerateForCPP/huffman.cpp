#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <iomanip>
#include <cstdio>
#include <queue>
#include <map>
#include <iterator>
#include <algorithm>
#include <cstring>
#include <string>
#include <sstream>

typedef std::map<std::string, int> fm_t;
 
const char* SampleString = "this is an example for huffman encoding";

static char const* keywords[] = {
    "\r\n"
    "bool",
    "break",
    "case",
    "char",
    "class",
    "const",
    "continue",
    "delete",
    "do",
    "double",
    "else",
    "enum",
    "false",
    "float",
    "for",
    "if",
    "int",
    "ling",
    "new",
    "operator",
    "or",
    "private",
    "protected",
    "public",
    "return",
    "short",
    "static",
    "struct",
    "switch",
    "template",
    "this",
    "true",
    "typedef",
    "typename",
    "unsigned",
    "virtual",
    "void",
    "while",
    NULL
};
 
typedef std::vector<bool> HuffCode;
typedef std::map<std::string, HuffCode> HuffCodeMap;
 
class INode
{
public:
    const int f;
 
    virtual ~INode() {}
 
protected:
    INode(int f) : f(f) {}
};
 
class InternalNode : public INode
{
public:
    INode *const left;
    INode *const right;
 
    InternalNode(INode* c0, INode* c1) : INode(c0->f + c1->f), left(c0), right(c1) {}
    ~InternalNode()
    {
        delete left;
        delete right;
    }
};
 
class LeafNode : public INode
{
public:
    std::string c;
 
    LeafNode(int f, std::string const& c) : INode(f), c(c) {}
};
 
struct NodeCmp
{
    bool operator()(const INode* lhs, const INode* rhs) const { return lhs->f > rhs->f; }
};
 
INode* BuildTree(fm_t const& frequencies)
{
    std::priority_queue<INode*, std::vector<INode*>, NodeCmp> trees;
 
    for(fm_t::const_iterator i = frequencies.begin(); i != frequencies.end(); ++i) {
        trees.push(new LeafNode(i->second, i->first));
    }
    while (trees.size() > 1)
    {
        INode* childR = trees.top();
        trees.pop();
 
        INode* childL = trees.top();
        trees.pop();
 
        INode* parent = new InternalNode(childR, childL);
        trees.push(parent);
    }
    return trees.top();
}
 
void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes)
{
    if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node))
    {
        outCodes[lf->c] = prefix;
    }
    else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node))
    {
        HuffCode leftPrefix = prefix;
        leftPrefix.push_back(false);
        GenerateCodes(in->left, leftPrefix, outCodes);
 
        HuffCode rightPrefix = prefix;
        rightPrefix.push_back(true);
        GenerateCodes(in->right, rightPrefix, outCodes);
    }
}

void readData(std::stringstream& p)
{
#ifdef DEBUG_HF
    freopen("Makefile", "r", stdin);
    FILE* f = stdin;
#else
    FILE* f = fdopen(0, "rb");
#endif
    while(!feof(f)) {
        union {
            struct {
                char c1, c2;
            };
            short s;
        } u;
        int read = getc(f);
        if(read == EOF) return;
        u.c1 = 0xFF & read;
        u.c2 = '\0';
        p << reinterpret_cast<char const*>(&u.s);
    }
}

void print(std::string const& s)
{
    if(isprint(s[0])) {
        std::cout << std::setw(10) << std::left;
        std::cout << s;
    } else {
        char const* p = s.c_str();
        std::cout << std::setw(10) << std::left;
        char temp[20];
        sprintf(temp, "0x%02X", *p);
        std::cout << temp;
    }
}

int main()
{
    // Build frequency table
    fm_t frequencies;
    std::stringstream input;

    readData(input);

    std::string ffs = input.str();
    int p = 0;

    while (p < ffs.size()) {
        for(int i = 0; keywords[i] != NULL; ++i) {
            if(strncmp(ffs.c_str() + p, keywords[i], strlen(keywords[i])) == 0) {
                fm_t::iterator f = frequencies.find(std::string(keywords[i]));
                if(f != frequencies.end()) {
                    f->second++;
                } else {
                    frequencies[std::string(keywords[i])] = 1;
                }
                p += strlen(keywords[i]) - 1;
                goto doneFor;
            }
        }
        if(iscntrl(ffs[p]) && !(ffs[p] == 0xa || ffs[p] == 0xd || ffs[p] == '\t')) { printf("hit %X\n", ffs[p]); p++; continue; }
        // nothing
        {
            std::stringstream s;
            assert(ffs[p]);
            char c = ffs[p];
            s << c;
            fm_t::iterator f = frequencies.find(s.str());
            if(f != frequencies.end()) {
                f->second++;
            } else {
                frequencies[s.str()] = 1;
            }
            p++;
        }
doneFor: ;
    }
 
    INode* root = BuildTree(frequencies);
 
    HuffCodeMap codes;
    GenerateCodes(root, HuffCode(), codes);
    delete root;
 
    for (HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it)
    {
        print(it->first);
        std::copy(it->second.begin(), it->second.end(),
                  std::ostream_iterator<bool>(std::cout));
        std::cout << std::endl;
    }
    return 0;
}
