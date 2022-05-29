curl -X POST "http://192.168.1.167/loop/" -d ''
curl -X POST "http://192.168.1.167/oneshot/" --data-binary @init.lua
sleep 1
curl -X POST "http://192.168.1.167/loop/" --data-binary @loop.lua
