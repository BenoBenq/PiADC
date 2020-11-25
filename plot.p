set term x11 1 noraise
set yrange [0:1023]
plot '< tail -n 10000 /home/bent/Coding/C/Pi/Project/recieve.d'
pause "0.1"
reread
