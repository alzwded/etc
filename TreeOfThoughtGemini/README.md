Played around trying to get Gemini Flash to use Tree Of Throught to see what happens

You can clone jakoestfilter and ask it to add a new filter which inverts lightness without affecting hue, it does that, then prompt it to do some other weird trick to what it just did (like "rewrite in C99 because the project is not C++", "optimize and add openmp", things like that).

Or even better, ask it to rewrite itself to work with opencode acp! (see opencode_cli.py)

The way it works:
- take a fast and cheap model and makes it slow and expensive

Okay, j/k, while true:
- add a "/tree" slash command
- ask a model (or, in different universes, perhaps multiple models of similar capabilities, but technically quirky in different ways) to generate N interpretations of the ask, then explore the project directory according to that world view
- ask a model (or blabla the same multiple models) to generate M implementation plans
- ask a model (preferably the most "wise") to judge the relevance of the whole plan to the original ask and interpretation, and give them a score
- pick the best plan (or pick top 3 and ask user; or maybe iterate on the plan definition a bit, possibly re-rolling the top 3 plans)
- execute plan

So what could be a couple of requests become 97 requests.

All in all: it takes a fast, cheap model, and makes it slow and expensive :-)

It was fun though.

Protip, pick the cheapest model:
- set GEMINI_MODEL=gemini-2.5-flash-lite or modify CLI invocation to --model gemini-2.5-flash-lite
- set COPILOT_MODEL=gpt-5-mini or modify CLI invocation to --model gpt-5-mini
- I don't know of any env var for opencode, so modify cli invocation to --model cheapest-fastest-thing-you-have-available
