空间 获取本机IP
//默认空间
function ss(url)
    http=com("Microsoft.XMLHTTP")
    http.Open("post",url, false)
    http.setRequestHeader("If-Modified-Since", "0")
    http.send("")
    while(http.ReadyState != 4)
        help.sleep(1)
    endwhile
    return http.responseText
endfunction
//开始按钮_点击操作
功能 开始_点击()
    ret=""
    temp=""
    i=0
while(ret!="ethernet")
        if(i<10)
            temp=0&i
            i++
        else
            temp=i
            i++
        endif
        paht="HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\00"&temp&"\\Ndi\\Interfaces"
	ret=系统.注册表获取键值(paht,"LowerRange")
        if(ret=="")
            help.messagebox("没有找到")
        endif
endwhile
IPpath="HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\00"&temp&"\\Linkage"
ipcode=系统.注册表获取键值(IPpath,"RootDevice")
ret=系统.注册表获取键值("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Interfaces\\"&ipcode,"IPAddress")
辅助.消息框(ret,"通过注册表获取")
功能结束

//退出按钮_点击操作
功能 退出_点击()
sru=获取本机IP.ss("http://iframe.ip138.com/ic.asp")
start=str.findstr(sru,"[")
end=str.findstr(sru,"]")
help.messagebox(str.strsub(sru,start+1,end),"通过系统对象xmlhttp获取")

功能结束

空间结束
//空间结束语句