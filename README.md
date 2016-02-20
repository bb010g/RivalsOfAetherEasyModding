# RivalsOfAetherEasyModding
This is just a personal project I made to make rivals of aether modding easier! Right now I only have sprite ripping and replacing but I plan to add more.

Current issues to avoid:
- If you don't have a sprite folder it will probably crash
- If you don't have a full sprite rip in your sprite folder when it tries to replace it will probably crash
- If you don't have an offsets.txt file it will probably crash
- If you rename the script it will probably crash (I think it doesn't if the length of the name is the same)

NOTE: I advise backing up your RivalsOfAether.exe before use!

How to use:
- Have a RivalsofAether.exe in the same folder as the script
- Have a sprite folder and a offsets.txt for the current update (I included one for 0.0.12 or whatever the current patch as of 2/20/2016 is)
- Have python 2 installed (Go here if you don't: https://www.python.org/downloads/ and download any version of python 2.7 and install it)
- Step 1: Open the script and type in 'rip' (no quotes) and hit enter, this will fill your sprites folder with the sprites ripped from the game
- Step 2: Edit the sprites in the sprites folder to your heart's content
- Step 3: Open the script again and type 'replace' (again no quotes)
- Step 4: Launch Rivals! Any sprites you edit will appear changed in game.


How to revert your changes:
- First, fill your sprites folder with vanilla sprites (which can be found here for 0.0.12: https://www.dropbox.com/s/odc4pl8njhqja9e/Sprite%20Rip.zip?dl=0)
- Replace again!

Also worth noting that if your sprites are modded it will extract the modded sprites when you rip from the exe.

Future plans:
- Fix any bugs people encounter
- Allow the modding of more things
- Have ripping find the offsets without a seperate file
