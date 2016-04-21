#Version 1.4 test
import Tkinter as tk
import tkMessageBox
import os
import time
import webbrowser
import subprocess
import thread
import binascii
import tkFileDialog
import glob
import shutil
import ttk

def getAppData():
    roamingPath = os.getenv('APPDATA')
    thereYet = False
    lengthBack = 0
    for i in range(0,len(roamingPath)):
        if not thereYet and roamingPath[len(roamingPath)-(i+1)] == '\\':
            thereYet = True
            lengthBack = len(roamingPath)-(i+1)

    return roamingPath[:lengthBack]

########PNG FORMAT###############
#Hex - 89 50 4E 47
#Dec - 137 80 78 71

#through

#Hex - 49 45 4E 44 AE 42 60 82
#Dec - 73 69 78 68 174 66 96 130
#################################

def ripSprite():
    try:
        subprocess.check_call(["RivalsEasyModdingC.exe","rip"])
        tkMessageBox.showinfo( "Finished", "Sprite rip complete.")
    except:
        tkMessageBox.showinfo( "Finished", "Error occurred.")

def replaceSprite():
    try:
        subprocess.check_call(["RivalsEasyModdingC.exe","replace"])
        tkMessageBox.showinfo( "Finished", "Sprite replacement complete.")
    except:
        tkMessageBox.showinfo( "Finished", "Error occurred.")

def update():
    try:
        subprocess.check_call(["RivalsEasyModdingC.exe","update"])
        tkMessageBox.showinfo( "Finished", "Offset update complete.")
    except:
        tkMessageBox.showinfo( "Finished", "Error occurred.")

def tutorial():
    webbrowser.open('youtu.be/Bg2Z2NyUp7c')

def about():
    webbrowser.open('https://github.com/jam1-garner/RivalsOfAetherEasyModding')

def run():
    thread.start_new_thread(subprocess.call,("RivalsofAether.exe",))

def ripWav():
    try:
        subprocess.check_call(["RivalsEasyModdingC.exe","ripWav"])
        tkMessageBox.showinfo( "Finished", "Audio rip complete.")
    except:
        tkMessageBox.showinfo( "Finished", "Error occurred.")

def replaceWav():
    try:
        subprocess.check_call(["RivalsEasyModdingC.exe","replaceWav"])
        tkMessageBox.showinfo( "Finished", "Audio replacement complete.")
    except:
        tkMessageBox.showinfo( "Finished", "Error occurred.")

def install():
    if tkMessageBox.askyesno("Install","Are you sure you want to install? Doing so will overwrite your current mod and modify your exe."):
        try:
            progressWindow = tk.Tk()
            text = tk.Label(progressWindow,text="Installing Mod...")
            text.pack()
            progress = ttk.Progressbar(progressWindow, orient="horizontal",length=200, mode="determinate")
            progress.pack()
            progressThread = thread.start_new_thread(progressWindow.mainloop,())
            progress["maximum"] = 1.0
            progress["value"] = 0.0
            appDataPath = getAppData()
            fileDirectory = tkFileDialog.askdirectory()
            path = os.path.realpath(__file__)[:len(os.path.realpath(__file__))-24]
            total = len(glob.glob(fileDirectory+"/sprites/RIP_*.png")) + len(glob.glob(fileDirectory+"/audio/RIP_*.wav")) + 5
            for f in glob.glob(fileDirectory+"/sprites/RIP_*.png"):
                for g in range(0,len(f)):
                    if f[g:g+7]=='sprites':
                        shutil.copyfile(f,path+f[g-1:])
                        progress["value"] += 1.0 / total
                        
            for f in glob.glob(fileDirectory+"/audio/RIP_*.wav"):
                for g in range(0,len(f)):
                    if f[g:g+5]=='audio':
                        shutil.copyfile(f,path+f[g-1:])
                        progress["value"] += 1.0 / total
                        
            for f in glob.glob(fileDirectory+"/dev_mode/custom_*.ini"):
                for g in range(0,len(f)):
                    if f[g:g+8]=='dev_mode':
                        shutil.copyfile(f,appDataPath+"/Local/RivalsofAether"+f[g-1:])

            if fileDirectory != '':
                replaceSprite()
                progress["value"] += 5.0 / total
                replaceWav()
                progress["value"] = 1
                tkMessageBox.showinfo("Install","Successfully installed.")
                progressWindow.destroy()
    
        except:
            tkMessageBox.showerror("Install","Failed to install.")

def backup():
    try:
        path = os.path.realpath(__file__)[:len(os.path.realpath(__file__))-24]
        shutil.copyfile(path+"RivalsofAether.exe",path+"RivalsofAetherBackup.exe")
        if not os.path.exists(path+"/backup/"):
            os.makedirs(path+"/backup/")
        shutil.copytree(path+"/sprites/",path+"/backup/sprites/")
        shutil.copytree(path+"/audio/",path+"backup/audio/")
        tkMessageBox.showinfo("Backup","Successfully backed up files.")
    except:
        tkMessageBox.showerror("Backup","Failed to backup files.")

#Main Stuff
path = os.path.realpath(__file__)[:len(os.path.realpath(__file__))-24]
if not os.path.exists(path+"/sprites/"):
    os.makedirs(path+"/sprites/")
if not os.path.exists(path+"/audio/"):
    os.makedirs(path+"/audio/")

top = tk.Tk()
menubar = tk.Menu(top)

filemenu = tk.Menu(menubar,tearoff=0)
filemenu.add_command(label="Rip Sprites",command=ripSprite)
filemenu.add_command(label="Replace Sprites",command=replaceSprite)
filemenu.add_separator()
filemenu.add_command(label="Rip Audio",command=ripWav)
filemenu.add_command(label="Replace Audio",command=replaceWav)
filemenu.add_command(label="Update Offsets",command=update)
filemenu.add_separator()
filemenu.add_command(label="Exit",command=top.quit)
menubar.add_cascade(label="File", menu=filemenu)

rivalsmenu = tk.Menu(menubar,tearoff=0)
rivalsmenu.add_command(label="Run RivalsofAether.exe",command=run)
rivalsmenu.add_command(label="Install Mod or Refresh from Backup",command=install)
rivalsmenu.add_command(label="Backup Files",command=backup)
menubar.add_cascade(label="Game",menu=rivalsmenu)

helpmenu = tk.Menu(menubar,tearoff=0)
helpmenu.add_command(label="Tutorial",command=tutorial)
helpmenu.add_command(label="About/Github",command=about)
menubar.add_cascade(label="Help",menu=helpmenu)

photo="""
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
"""

photo = tk.PhotoImage(data=photo)

label = tk.Label(image=photo)
label.image = photo
label.pack()

top.wm_title("Rivals of Aether Modding Tool")
top.config(menu=menubar)
top.mainloop()
