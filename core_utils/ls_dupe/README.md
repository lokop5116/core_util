# List 


Compile normally using gcc ( or any other compiler of your preference ):

```bash
gcc ls.c -o ls_dupe
```


So far ( and for the near future ) only supports -a flag. Use -h to display the following help message.

LS_Dupe - this is a ( attempt at ) rewriting the GNU ls util.
Intended to work the same as GNU ls. Supports only -a flag. See usage below.

ls [-a] [path]

        -a,      do not ignore entries starting with '.'
        -h,      display this help message
