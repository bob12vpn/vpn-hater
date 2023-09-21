# tls-hello
capture tls.handshake.extensions_server_name

# display filter
```
    eth.type == 0x0800
and (ip.proto == 6 or ip.proto == 17)
and tcp.dstport == 443
and tls.record.content_type == 22
and tls.handshake.type == 1
and tls.handshake.extension.type == 0
```

### filter for copy
```
eth.type == 0x0800 and (ip.proto == 6 or ip.proto == 17) and tcp.dstport == 443 and tls.record.content_type == 22 and tls.handshake.type == 1 and tls.handshake.extension.type == 0
```

# filter code explanation
### code
```
        if(!isTLS) continue;
		if(offset == ntohs(ip_hdr->ip_len) + ETH_HDR_LEN) continue;	// for filter handshake with garbage value
```

### before
![image](https://github.com/bob12vpn/tls-hello/assets/47083922/a62d2116-3e71-499f-b95a-d6d21cfeeb14)

### after
![image](https://github.com/bob12vpn/tls-hello/assets/47083922/862f6b73-ea7e-4ca7-a217-2337f1b2974e)

packet 변수를 동적 할당을 하지 않는 이상 뒤에 이전 패킷의 값이 그대로 남아버려서 더 많이 출력되는 버그가 발생

이를 해결하기 위한 필터
