from PIL import Image
import numpy as np
import logging as log


log.basicConfig(
    format="%(asctime)s %(levelname)s: %(message)s",
    datefmt="%Y-%m-%dT%H:%M:%SZ %Z",
    level=log.INFO,
    filename="image_playfair.log",
    force=True
)


class ImagePlayfair:
    def __init__(self, EXTRA_DIFFUSE=True):
        self.EXTRA_DIFFUSE = EXTRA_DIFFUSE

    def encrypt(self, keyImage, inputImage):
            self.__generateKeyMatrix(keyImage)
            R,G,B,A,sizes=self.__getRGB(inputImage, False)
           
            encryptedR=np.full(len(R),-1)
            encryptedG=np.full(len(G),-1)
            encryptedB=np.full(len(B),-1)

            
            halfLength = len(R)//2
            for i in range(halfLength):
                self.__encryptDecryptPixel(encryptedR,i,halfLength, self.RkeyMatrix,R)
                self.__encryptDecryptPixel(encryptedG,i,halfLength, self.GkeyMatrix,G)
                self.__encryptDecryptPixel(encryptedB,i,halfLength, self.BkeyMatrix,B)
        
           
                self.__encryptDecryptPixel(encryptedR,i,halfLength, self.GkeyMatrix,R, False)
                self.__encryptDecryptPixel(encryptedG,i,halfLength, self.BkeyMatrix,G, False)
                self.__encryptDecryptPixel(encryptedB,i,halfLength, self.RkeyMatrix,B, False)    
        
                self.__encryptDecryptPixel(encryptedR,i,halfLength, self.BkeyMatrix,R, False)
                self.__encryptDecryptPixel(encryptedG,i,halfLength, self.RkeyMatrix,G, False)
                self.__encryptDecryptPixel(encryptedB,i,halfLength, self.GkeyMatrix,B, False)  
            
            fullImage1D=[]
            encryptedB = np.concatenate((encryptedB[halfLength:],encryptedB[:halfLength]))
            encryptedG = np.concatenate((encryptedG[halfLength:],encryptedG[:halfLength]))
            encryptedR = np.concatenate((encryptedR[halfLength:],encryptedR[:halfLength]))

           
            n = len(encryptedR) - 1
            for i in range(len(encryptedR)):
                fullImage1D.append([encryptedR[n - i],encryptedG[i],encryptedB[n - i],A[i]])
            
            x,y,z=sizes[0],sizes[1],4
            imageArr = np.reshape(fullImage1D,(y,x,z))

            image = Image.fromarray(imageArr.astype('uint8'),'RGBA')
            image.save('enc.png')
    
    

    def decrypt(self, keyImage, inputImage):
        self.__generateKeyMatrix(keyImage)
        R,G,B,A,sizes=self.__getRGB(inputImage,False)

        decryptedR=np.full(len(R),-1)
        decryptedG=np.full(len(G),-1)
        decryptedB=np.full(len(B),-1)

      
        


        halfLength = len(R)//2
        for i in range(halfLength):
            
            self.__encryptDecryptPixel(decryptedR,i,halfLength, self.RkeyMatrix,R,False)
            self.__encryptDecryptPixel(decryptedG,i,halfLength, self.GkeyMatrix,G,False)
            self.__encryptDecryptPixel(decryptedB,i,halfLength, self.BkeyMatrix,B,False)

            self.__encryptDecryptPixel(decryptedR,i,halfLength, self.GkeyMatrix,R)
            self.__encryptDecryptPixel(decryptedG,i,halfLength, self.BkeyMatrix,G)
            self.__encryptDecryptPixel(decryptedB,i,halfLength, self.RkeyMatrix,B)

            self.__encryptDecryptPixel(decryptedR,i,halfLength, self.BkeyMatrix,R)
            self.__encryptDecryptPixel(decryptedG,i,halfLength, self.RkeyMatrix,G)
            self.__encryptDecryptPixel(decryptedB,i,halfLength, self.GkeyMatrix,B)

           
           
            
        fullImage1D=[]

        decryptedB = np.concatenate((decryptedB[halfLength:],decryptedB[:halfLength]))
        decryptedG = np.concatenate((decryptedG[halfLength:],decryptedG[:halfLength]))
        decryptedR = np.concatenate((decryptedR[halfLength:],decryptedR[:halfLength]))

        n = len(decryptedR) - 1
        for i in range(len(decryptedR)):
            fullImage1D.append([decryptedR[n - i],decryptedG[i],decryptedB[n - i],A[i]])

        x,y,z=sizes[0],sizes[1],4
        imageArr = np.reshape(fullImage1D,(y,x,z))
        image = Image.fromarray(imageArr.astype('uint8'),'RGBA')
        image.save('original.png')
    
    
    
        

    def __generateKeyMatrix(self, filename):
            R,G,B,A,s=self.__getRGB(filename,True)
            R=self.__removeDuplicates(R)
            G=self.__removeDuplicates(G)
            B=self.__removeDuplicates(B)
            A=self.__removeDuplicates(A)
            R.extend([x for x in range(256) if x not in R])
            self.RkeyMatrix = np.reshape(R, (16,16))
            G.extend([x for x in range(256) if x not in G])
            self.GkeyMatrix = np.reshape(G, (16,16))
            B.extend([x for x in range(256) if x not in B])
            self.BkeyMatrix = np.reshape(B, (16,16))
            
            
            
    def __removeDuplicates(self, array):
        newArr = []
        for i in array:
            if i not in newArr:
                newArr.append(i)
        return newArr

    def __getRGB(self, filename,resize=False):
        img = Image.open(filename)
        if resize:
            size = 16,16
            img = img.resize(size,Image.Resampling.LANCZOS)
            img= img.convert('RGB')
        numpydata = np.asarray(img)
        R=[]
        G=[]
        B=[]
        A=[]
        for row in numpydata:
            for pixel in row:
                R.append(pixel[0])
                G.append(pixel[1])
                B.append(pixel[2])
                try:
                    A.append(pixel[3])
                except IndexError:
                    A.append(255)
                    pass
        return (R,G,B,A,img.size)
    
    def __encryptDecryptPixel(self, encryptResult,i,halfLength,colorKeyMatrix,color,isEncryption=True):
        floc = self.__findLoc(colorKeyMatrix,color[i])
        sloc = self.__findLoc(colorKeyMatrix,color[halfLength+i])
        if isEncryption:
            if self.EXTRA_DIFFUSE:
                floc[0]=(floc[0]+9)%16
                floc[1]=(floc[1]+9)%16
                sloc[0]=(sloc[0]+9)%16
                sloc[1]=(sloc[1]+9)%16
            if floc[0] == sloc[0]:
                encryptResult[i]=colorKeyMatrix[floc[0],(floc[1]+1)%16]
                encryptResult[halfLength+i] = colorKeyMatrix[sloc[0],(sloc[1]+1)%16]
            elif floc[1] == sloc[1]:
                encryptResult[i]=colorKeyMatrix[(floc[0]+1)%16,floc[1]]
                encryptResult[halfLength+i] = colorKeyMatrix[(sloc[0]+1)%16,sloc[1]]
            else:
                encryptResult[i]=colorKeyMatrix[floc[0],sloc[1]]
                encryptResult[halfLength+i] = colorKeyMatrix[sloc[0],floc[1]]
        else:
            if self.EXTRA_DIFFUSE:
                floc[0]=(floc[0]+7)%16
                floc[1]=(floc[1]+7)%16
                sloc[0]=(sloc[0]+7)%16
                sloc[1]=(sloc[1]+7)%16
            if floc[0] == sloc[0]:
                encryptResult[i]=colorKeyMatrix[floc[0],(floc[1]+15)%16]
                encryptResult[halfLength+i] = colorKeyMatrix[sloc[0],(sloc[1]+15)%16]
            elif floc[1] == sloc[1]:
                encryptResult[i]=colorKeyMatrix[(floc[0]+15)%16,floc[1]]
                encryptResult[halfLength+i] = colorKeyMatrix[(sloc[0]+15)%16,sloc[1]]
            else:
                encryptResult[i]=colorKeyMatrix[floc[0],sloc[1]]
                encryptResult[halfLength+i] = colorKeyMatrix[sloc[0],floc[1]]
        
          
    def __findLoc(self, mat,x):
        coordinates = np.where(mat==x)
        return [coordinates[0][0],coordinates[1][0]]
        


if __name__ == "__main__":
    imagePlayfair = ImagePlayfair()
    key = input("Enter the key image: ")
    inputImage = input("Enter the input image: ")
    imagePlayfair.encrypt(key, inputImage)
    log.info("Encrypted image saved as enc.png")
    imagePlayfair.decrypt(key, "enc.png")
    log.info("Decrypted image saved as original.png")