Based on https://github.com/cnlohr/nosdk8266


## Tips I Learned

1. GCC Xtensa will create smaller code if you have volatile pointers to arrays, rather than setting absolute memory addresses.  Indirect accessing isn't slower, and it makes smaller code.  Additionally, if you just store the pointer to the base of an array, like the IO MUX register, and you index into it multiple times, it doesn't need to take up prescious space holding that.

2. Avoid using macro's to do pointer arithmatic.  Also, try to find out where you are or'ing masks, etc. where you don't need to be.

3. Always make sure to have your function declarations available when used.  Failure to do this gimps -flto's ability to inline functions.

4. Compile with -g to make the assembly listing much easier to read. 

I'm sure there's more...
# Todo

* Figure out how to get rid of the prologue to main().  Even though it's marked as noreturn, GCC is doing stuff with the stack, etc.

* Add Sleep feature.

* Figure out why power consumption is higher than I would expect.
