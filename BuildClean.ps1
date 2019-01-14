start-transcript build.log
cd \Users\bstar\documents\github\2616j\tp2616j-red
prosv5 make clean
prosv5 make
prosv5 upload --slot 1 --name Red-Front  --no-run-after
cd \Users\bstar\documents\github\2616j\tp2616j-Blue
prosv5 make clean
prosv5 make
prosv5 upload --slot 2 --name Blue-Front  --no-run-after
cd \Users\bstar\documents\github\2616j\tp2616j-RedBack
prosv5 make clean
prosv5 make
prosv5 upload --slot 3 --name Red-Back  --no-run-after
cd \Users\bstar\documents\github\2616j\tp2616j-BlueBack
prosv5 make clean
prosv5 make
prosv5 upload --slot 4 --name Blue-Black  --no-run-after
cd \Users\bstar\documents\github\2616j\tp2616j-RedSkills
prosv5 make clean
prosv5 make
prosv5 upload --slot 5 --name Skills  --no-run-after
cd \Users\bstar\documents\github\2616j\tp2616j-blueplat
prosv5 make clean
prosv5 make
prosv5 upload --slot 6 --name BPlat  --no-run-after
stop-transcript
invoke-item \Users\bstar\documents\github\2616j\build.log
