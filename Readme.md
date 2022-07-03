
# Obenkyobo

```diff
- Please be aware, this application is still early in its life cycle.
- Turning off your device will not put it to sleep, it will only turn off the light.
```

## What is it ?

Obenkyobo is an application for kyobo ereaders designed to help you memorize Hiraganas, Katakanas and vocabulary. 

## How does it look ?
<p align="center">
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/about.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/hiragana1.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/hiragana2.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/katakana2.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/hiraganaEdit.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/hiraganaEdit2.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/VocabDir.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/vocabLearn1.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/vocabLearn2.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/SettingsApplication.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/SettingsFonts.png" width="252" height="320" >
  <img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/SettingsScreen.png" width="252" height="320" >
</p>
More screenshots : <a href="https://imgur.com/a/8ehpdbf">here (imgur gallery)</a>

## Features ?

Here's what available currently :
- Hiragana/Katakana <-> Romanji customizable MCQ
- Vocabulary sheet system (create your owns sheets, hide columns, hide cells, randomize content, change number of row per page, etc)
- Weighted random / "learning state" for each Hiragana/Katakana to keep MCQ more entertaining (of course, progress is stored in between each session)
- Quality of Life features such a; multiple fonts, font settings, screen options (Luminosity/Tint) support, physical buttons support, etc
- Wifi support (kind of)

Tested on : 
- Libra h2o
- Forma
- Mini (might have minor scaling issues)
- Glo
- Glo HD
- More incoming, please report if it works for your model too

## How to install ?

Firstly, this application is designed to be compatible with most (except some new models) Kobo devices with a recent firmware (probably 4.6+). Do not try to install it for other ereader, it will not work ! This application is provided "as is". Don't come to me if somehow your device catch on fire.

Secondly, you'll need to install a "launcher" beforehand. Common options are : 
- <a href="https://github.com/pgaskin/NickelMenu">NickelMenu (NM)</a>, recommended option
- <a href="https://github.com/NiLuJe/kfmon">Kute File Monitor (KfMon)</a>

Once you're done installing at least one of those you can :
- download the archive  <a href="https://github.com/Aryetis/Obenkyobo/releases">Obenkyobo_AIO_Release_V*.*.*.zip</a> 
- connect the ereader to your computer and extract the package at its root.

 It should work "as is" assuming you're using NickelMenu. If you're using KfMon you'll have to uncomment the associated entry file (`.adds/kfmon/config/Obenkyobo.ini`) by removing one of the `;` located at the start of each line. And you might want to delete the useless NickelMenu entry too `.adds/nm/Obenkyobo`

If you're only updating Obenkyobo, you can install the "light" version of the archive instead.  <a href="https://github.com/Aryetis/Obenkyobo/releases">Obenkyobo_Light_Release_V*.*.*.zip</a>.  This version is stripped from qt packages you already installed with the previous version and should work fine for updating except if told otherwise.

<img src="https://git.hijackedbrain.com/hijackedbrain/Obenkyobo/raw/branch/master/Screenshots/NickelMenu.png" width="303" height="184" align="left"/>
<br/>
<br/>
<br/>
And voilà ! You should now have a NickelMenu entry at the bottom right corner of your ereader "home screen" and/or a new "book" in your library to open with KfMon.
<br clear="left"/>

## How to create my vocabulary sheets ?

Each ****.oben file  in `Obenkyobo/vocab` is a Vocabulary sheet.<br/> (default ones are in french, I encourage you to write your owns)
To make your own, create a file and follow the same template as the default ones.

Each line should follow this format : `[fontType=hiragana][jp=いぬ][kanji=犬][trad=chien][learningScore=5]`

You can insert commentary line by having it start with a `#`.

Possible values for fontType are `hiragana, katakana`, it will determine what fonts is used in Obenkyobo to display this entry.

LearningScore value range [0;5], please note that the value is actually inversed from what is displayed in Obenkyobo. And the LearniningScore attribute of vocabulary sheets will be used when I'm done implementing vocabulary MCQ.

If your file is misinterpreted in Obenkyobo, please make sure your text editor (notepad++, sublimeText, etc) is saving the file in UTF-8 format.

## Issues ? 

If you encounter any issues, please report them by either : 
- Creating a new Ticket on Github <a href="https://github.com/Aryetis/Obenkyobo/issues">here</a> describing your issue as precisely as possible, include the type of ereader your using and the log.txt at `/.adds/Obenkyobo/log.txt` (it erases itself at each launch, so don't restart Obenkyobo right after a crash)
- Respond in Obenkyobo's MobileRead thread <a href="https://www.mobileread.com/forums/showthread.php?p=4162746">MobileRead thread</a> in the same way.


## Thanks

Thank you for actually reading a readme file.

And big big thanks to <a href="https://github.com/Rain92/">@Rain92/OfficerAction</a> , <a href="https://github.com/NiLuJe">@NiLuJe</a> and <a href="https://github.com/tux-linux">@NiMa</a> for putting up with me on Discord helping me setup my environment, walking me throught some Qt Kobo specific minefields and creating all those nice tools that allowed me to develop this app.

## For developpers, dummies, and future me. How to compile it ?

Please check the [DevReadme.md ](DevReadme.md) file.

