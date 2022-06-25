How to Use Exuberant CTags
==========================

In the interest of not spending 15' each time I want to remember how to do it:

    ctags --extra=+fq --fields=+amnStail --c-kinds=+cdefglpx --c++-kinds=+lpx -f tags -R .

In `.vimrc`,

    :set tags=./tags,tags,/home/me/tags
  
That means seach 1) in the same directory as the source, 2) in the cwd, 3) in ~me. What I usually have is:

    :set tags=/local/tags,/group/tags,/fullbuild/tags
  
Vim commands, in the order I remember them in:

    CTRL-]             equivalent to :tag; jumps there; kinda like :tjump
    :tselect           list definitions from tags
    :stselect          like :tselect, but in a new windo
    g CTRL-]           like CTRL-] but use :tjump
    g]                 performs :tselect on thing under cursor
    :help tagsrch.txt  :-)
  
Fun stuff: tselect accepts a regex.

**Note**: the old UNIX ctags only does the basic C stuff.

    ctags -f tags *.c *.h *.f             # create tags file
    ctags -a -f tags *.c                  # append to tags file
    ctags -x *.c *.h *.f                  # kinda like a source-level version of nm -C or smth like that
                                          # in exuberant CTags is kinda plays dumb and it sticks to C/FOR
                                          # `x' is for "cross-reference file" :-) Yeah, that sounds like
                                          # something from the FORTRAN days.

**Update**: I had already figured it out [here](./generateCtags.sh). Let's just duplicate everything, maybe I'll remember it

```bash
#!/bin/sh

# --tag-relative=yes  <-- files in tags are relative to tags file and not to cwd
# --fields: enable everything I know of
# --extra: extra stuff for C++
# --c&c++-kinds: enable everything I know of
ctags -f tags --tag-relative=yes --fields=+a+f+i+k+l+m+n+s+S+z+t --extra=+f+q --c-kinds=+c+d+e+f+g+l+m+n+p+s+t+u+v+x --c++-kinds=+c+d+e+f+g+l+m+n+p+s+t+u+v+x -R .

echo 'vim usage:
    g]              tselect `.
    :tselect [s]    show a pick list of tag definitions
    :tag [s]        go to first matching tag
    :tn
    :tp'
```

cscope
======

This is also a cool tool.

Use `cscope -bqR` to (re)generate db.

Use `cscope -bqR -s../include -s../drv` for additional sources. Add `-u` to force rebuild.

Vim runs it as a piped child process.

Start with `:cs add cscope.out`. Check processess with `:cs show`. I'm not sure how to globally generate this for a 3+ tier build system o_O.

In vim, the `:cs` command is the entrypoint which shows help. Examples

    :cs find g symbol_to_lookup
    :cs find c callable
    :cs find d what_does_this_call
    :cs find s C_symbol
    :cs find t text_string
    :cs find e egrep_pattern
    :cs find f file
    
It looks pretty useful, if a bit more dumb than ctags. The `c` thing seems pretty cool. Although it shows a lot of usages and whatnot. This together with ctags should be awesome as it covers everyone's need. Now, it's a shame that it appears to only work with C/C++.

It has caveats for C++.

GNU GLOBAL
==========

I haven't figured it out. Use `gtags` to generate 3 files, then `global` (with a ton of arguments) to look stuff up. Apparently there's a Vim plugin, but it wasn't already installed so I didn't bother.

`global -x thing` prints in the `ctags -x` format`.

Does this thing literally only find definitions period?!?!?!
