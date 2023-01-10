import warp as wp
import sys
import numpy as np
from PIL import Image, ImageFilter

# The general steps are:
# 1. Initialise Warp
# 2. Load the data
    # 1. For imaging applications, you need to get the image mode, so you know whether its a grayscale image
    # or a colour image
    # 2. You will need to use a different kernel depending on the image type
# 3. Convert the input data image into a NumPy array
# 4. Convert a NumPy array into a Warp array
# 5. Create an empty Warp array for the output image and initialize it to zero
# 6. Launch the kernel - which needs to be written separately
# 7. Convert the output Warp array to NumPy
# 8. Create the output data (e.g. a PIL Image) from a NumPy array
# 9. Save the output data (e.g. PIL image) to disk



def main():
    wp.init()  #step 1
    device = "cpu"

    @wp.kernel
    def greyscaleFilterSharp(input: wp.array2d(dtype=wp.float32), 
        output: wp.array2d(dtype=wp.float32), param: wp.float32, shape0: wp.int32, shape1: wp.int32):
            i, j = wp.tid()
            output[i,j] = input[i,j]
            rad = int(3)
            counter = int(0)
            mean = float(0.0)
            blur = float(0.0)
            #if( (i - int(rad/2)) >= 0 and (j - int(rad/2)) >= 0 and (i + int(rad/2)) < shape0 and (j + int(rad/2)) < shape1):
            for c in range(int(-(rad/2)), int(rad/2) + 1):
                for d in range(int(-(rad/2)), int(rad/2) + 1):
                    if( (i + c) >= 0 and (j + d) >= 0 and (i + c) < shape0 and (j + d) < shape1):
                        mean = mean + input[i + c, j + d]
                        counter = counter + 1
            blur = mean / float(counter)
            output[i,j] = input[i,j] + (param * (input[i,j] - blur))
                    
                
            

    @wp.kernel
    def greyscaleFilterNRem(input: wp.array2d(dtype=wp.float32), 
        output: wp.array2d(dtype=wp.float32), rad: wp.int32, shape0: wp.int32, shape1: wp.int32):
            i, j = wp.tid()
            output[i,j] = input[i,j]
            mean = float(0.0)
            counter = int(0)
            for c in range(int(-(rad/2)), int(rad/2) + 1):
                for d in range(int(-(rad/2)), int(rad/2) + 1):
                    if( (i + c) >= 0 and (j + d) >= 0 and (i + c) < shape0 and (j + d) < shape1):
                        mean = mean + input[i + c, j + d]
                        counter = counter + 1
            output[i,j] = mean/float(counter)
         

    @wp.kernel
    def RGBSharp(input: wp.array3d(dtype=wp.float32), 
        output: wp.array3d(dtype=wp.float32), param: wp.float32, shape0: wp.int32, shape1: wp.int32):
            i, j, k = wp.tid()
            output[i,j,k] = input[i,j,k]
            rad = int(3)
            mean = float(0.0)
            blur = float(0.0)
            counter = int(0)
            for c in range(int(-(rad/2)), int(rad/2) + 1):
                for d in range(int(-(rad/2)), int(rad/2) + 1):
                    if( (i + c) >= 0 and (j + d) >= 0 and (i + c) < shape0 and (j + d) < shape1):
                        mean = mean + input[i + c, j + d, k] 
                        counter = counter + 1
            blur = mean / float(counter)
            output[i,j,k] = input[i,j,k] + (param * (input[i,j,k] - blur))

            if output[i,j,k] > 255.0:
                output[i,j,k] = 255.0
            if output[i,j,k] < 0.0:
                output[i,j,k] = 0.0

    @wp.kernel
    def RGBNRem(input: wp.array3d(dtype=wp.float32), 
        output: wp.array3d(dtype=wp.float32), rad: wp.int32, shape0: wp.int32, shape1: wp.int32):
            i, j, k = wp.tid()
            output[i,j,k] = input[i,j,k]
            mean = float(0.0)
            counter = int(0)
            for c in range(int(-(rad/2)), int(rad/2) + 1):
                for d in range(int(-(rad/2)), int(rad/2) + 1):
                    if( (i + c) >= 0 and (j + d) >= 0 and (i + c) < shape0 and (j + d) < shape1):
                        mean = mean + input[i + c, j + d, k] 
                        counter = counter + 1         
            output[i,j,k] = mean/float(counter)

            if output[i,j,k] > 255.0:
                output[i,j,k] = 255.0
            if output[i,j,k] < 0.0:
                output[i,j,k] = 0.0


                


    if len(sys.argv) < 6:
        print("Invalid Usage\nUsage: python3 a3.py algType kernSize param inFile outFile")
        exit(0)
    algType = sys.argv[1]
    if algType != "-n" and algType != "-s":
        print("Algorithm type MUST be '-n' or '-s'")
        exit(0)
    kernSize = int(sys.argv[2])
    if algType == "-n":
        if kernSize % 2 == 0 or kernSize <= 0:
            print("Kernel size must be positive and odd")
            exit(0)
    param = float(sys.argv[3])
    inFile = sys.argv[4]
    outFile = sys.argv[5]
    image = Image.open(inFile) #2 load the data and get image type
    #image = image.convert("L")
    
    
    #print(imgType)

    numpyArr = np.asarray(image, dtype=float) #3 convert input to numpy
    #print(numpyArr.shape)
    inWarpData = wp.array(numpyArr, dtype=float, device=device) #4 numpy to warp
    outWarpImage = wp.zeros(shape=numpyArr.shape, dtype=float,device=device)
    imgType = image.mode

    if imgType == "L": #grayscale 
        if(algType == "-s"):
            wp.launch(kernel=greyscaleFilterSharp,
                dim = numpyArr.shape,
                inputs=[inWarpData,outWarpImage, param, numpyArr.shape[0], numpyArr.shape[1]],
                device=device)
        if(algType == "-n"):
            wp.launch(kernel=greyscaleFilterNRem,
                dim = numpyArr.shape,
                inputs=[inWarpData,outWarpImage, kernSize, numpyArr.shape[0], numpyArr.shape[1]],
                device=device)

    elif imgType == "RGB" or imgType == "RGBA": #RGB 
         if(algType == "-s"):
             wp.launch(kernel=RGBSharp,
                 dim = numpyArr.shape,
                 inputs=[inWarpData,outWarpImage, param, numpyArr.shape[0], numpyArr.shape[1]],
                 device=device)
         if(algType == "-n"):
             wp.launch(kernel=RGBNRem,
                 dim = numpyArr.shape,
                 inputs=[inWarpData,outWarpImage, kernSize, numpyArr.shape[0], numpyArr.shape[1]],
                 device=device)


    numpyOutArr = outWarpImage.numpy()
    imageOut = Image.fromarray(np.uint8(numpyOutArr))
    open(outFile, 'w')
    imageOut.save(outFile)

main()