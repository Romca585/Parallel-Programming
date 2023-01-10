
Disclaimer: I had some issues running this on docker using the cpu, thus the school socs server was used.

The description asked for a makefile, I'm pretty sure this is a typo, considering that python does not require
a makefile - stating this just in case.

Takes in an image as the input - the program will run differently with BW and RGB/RGBA images




Usage: python3 a3.py algType kernSize param inFile outFile

    Each argument is as per the assignment description - thus I won't describe what each argument is.

    Error handling is present, such that if the algType does not match or other issues with the arguments are present
    the program will let you know and exit

    If algType is -n, the important variable is kernSize which as per the assignment description
    has to be positive (0 is not allowed) and a multiple of 3. You can set the param as anything, it
    will not be used

    If algType is -s, the important variable is param which can be any value. I found that using a 
    negative value gives a cool effect, so I did not error handle it. You can leave kernSize as
    any value, as it is only error handled when the algType is -n


    I found the most appropriate values for kernSize to be 3,5 and 7
    I found the most apporpriate values for param to be between 0.2 and 2.0


Algorithms implemented:
    As per Denis' later revision, he allowed us to reuse the same algorithms for both Noise Reduction
    and Sharpening. Thus, for both tasks I used the Mean Filtering. This was because it is impossible
    to make a local array to store the median values to calculate the median.

    Clarification for sharpening:
        I used Mean Filtering with a radius of 3 assigning it to a blurred pixel variable
        And then used the unsharp masking formula to complete the sharpening. 


How the kernel was implemented + border handling:
    I used 4 kernels total 2 kernels for each RGB and Grayscale
    
    Each kernel is very similar in the sense that when calculating the mean,
    it checks to see if the neighbour in question is in range of the image's shape
        If it is - that neighbour is added to the mean and a counter is incremented
            to indicate the neigbour was added
        If it is not - the neigbour is ignored
    At the end the mean of the valid neigbours is calculated and assigned to the pixel being
    worked on.

    Example of a kernel from the code:

1        @wp.kernel
2        def greyscaleFilterNRem(input: wp.array2d(dtype=wp.float32), 
3            output: wp.array2d(dtype=wp.float32), rad: wp.int32, shape0: wp.int32, shape1: wp.int32):
4                i, j = wp.tid()
5                output[i,j] = input[i,j]
6                mean = float(0.0)
7                counter = int(0)
8                for c in range(int(-(rad/2)), int(rad/2) + 1):
9                    for d in range(int(-(rad/2)), int(rad/2) + 1):
10                       if( (i + c) >= 0 and (j + d) >= 0 and (i + c) < shape0 and (j + d) < shape1):
11                           mean = mean + input[i + c, j + d]
12                           counter = counter + 1
13               output[i,j] = mean/float(counter)

    lines 1-3 define the kernel - shape0 and shape1 sends in the x,y coordinates of the shape of the image, which is used to define the border
        rad is the radius that was sent it (in this case kernSize - which means a radius of 3)
    
    lines 8 and 9 will give values c and d such that when added to i and j respectively will give the neighbours of input[i,j]
        c and d can also be negative values (obviously adding a neg value to a pos is the same as subtraction)
    
    line 10 checks to see if the neighbour is valid: such that if either i or j is either below 0 or above the max shape of the image
        that pixel is ignored.

    If the test in line 10 passes, lines 11 and 12 will add that pixel to the mean to be calculated and the counter will be incremented
    Finally line 13 calculates the mean and assigns it to the pixel being worked on


    Verify how well this algorithm works by setting the kernSize to a ridiculous number such as 81 where literally half of the image, otherwise would not
    be processed.
    In the case where kernSize is not used (sharpening) - the radius is 3, and for the most part the portion of the image that requires border handling is very
    small. 






















        
