# Project-DIVA-Corruption
A tool to completely randomise the modules and target charts in PDX.

# How to use
This tool has two uses: the first is randomising the firstread.bin, the other is randomising all DSCs
fr - randomises the decrypted firstread.bin in the same location as the exe. Saves the randomised one in PDRX Files/firstread.bin
dsc - randomises all encrypted DSCs in the same location as the exe or all decrypted DSCs in the Decrypted_PDX_DSCs folder from a previous use. Saves all randomised DSCs in PDRX Files.

# Randomising the firstread.bin
To randomise the firstread.bin you will need to get the DIVAFILE_Tool.exe located [here](https://github.com/s117/DIVAFILE_Tool/releases)

Follow the instructions on the [readme](https://github.com/s117/DIVAFILE_Tool/blob/master/README.md) and decrypt your firstread.bin file (You only ever have to do this once!). 

Now place both the DIVAFILE_Tool.exe (for the DSCs later) and firstread.bin in the same location as the PDRX.exe

Run the PDRX.exe in the command prompt with the argument "fr" (without quotations).

Your randomised firstread.bin will be saved in the PDRX Files folder in the same location

# Randomising the DSCs
Get the DSCs you want to randomise from the data.psarc and place them in the same location as the exe.

Make sure the DIVAFILE_Tool.exe is also in the same location as the exe and DSCs

Run the PDRX.exe in the command prompt with the argument "dsc" (without quotations).

Now you will have 2 new folders. One folder called Decrypted_PDX_DSCs which contains all of the decrypted DSCs you have next to the PDRX.exe. This allows you to delete the encrypted DSCs next to the PDRX.exe and instead use these ones. 

The other folder called PDRX Files will contain the randomised DSCs.

# Known Bugs
Sometimes the face of a module will not show up, currently unsure why this is happening at this time but not game breaking thankfully.

Within a randomised chart, sometimes a hold will appear but have no end which will cause you to MISS every other target. This makes the chart unplayable and will have to be re-randomised. Working on a fix for this issues currently.

Finally, a module may infinitly be stuck loading and cause the game to crash (both on the module selection screen and when loading up a song with said broken module). I know why this bug is caused and am trying to find a work around for it.

Any other bugs, feel free to contact me!
