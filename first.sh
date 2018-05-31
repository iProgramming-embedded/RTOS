#!/bin/bash

 git add .
 git commit -m "8th"
 git remote rm origin
 git remote add origin git@github.com:LeonLinuxNerd/RTOS.git
 git push -u origin master
