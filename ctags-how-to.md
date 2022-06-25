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
  :help tagsrch.txt  :-)
  
Fun stuff: tselect accepts a regex.

**Note**: the old UNIX ctags only does the basic C stuff.

  ctags -f tags *.c *.h *.f             # create tags file
  ctags -a -f tags *.c                  # append to tags file
  ctags -x *.c *.h *.f                  # kinda like a source-level version of nm -C or smth like that
                                        # in exuberant CTags is kinda plays dumb and it sticks to C/FOR
                                        # `x' is for "cross-reference file" :-) Yeah, that sounds like
                                        # something from the FORTRAN days.
