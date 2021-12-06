# Test code for socket programming (UDP)

## Linuxコマンドでデバッグ
### WSLのIPアドレスの調べ方
- `ip adress`
- 下記の場合だと、172.20.53.171

```ip address
6: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP group default qlen 1000
    link/ether 00:xx:xx:xx:xx:xx brd ff:ff:ff:ff:ff:ff
    inet 172.20.53.171/20 brd 172.20.63.255 scope global eth0
```

### デバッグコマンド
- 受信 (port=1234で受信)
    - `nc -lu 1234`
- 送信 (address=192.168.1.2(Windows)の、port=5678に送信)
    - `nc -u 192.168.1.2 5678`


