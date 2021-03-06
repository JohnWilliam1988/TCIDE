空间 获取本机IP
//默认空间
// 利用系统组件获取本机外网的IP
function 外网的IP(url)
    // 创建插件对象
    http=com("Microsoft.XMLHTTP")
    // 提交网页连接
    http.Open("post",url, false)
    http.setRequestHeader("If-Modified-Since", "0")
    http.send("")
    // 等待网页加载完成
    while(http.ReadyState != 4)
        help.sleep(1)
    endwhile
    // 返回网页内容
    return http.responseText
endfunction

//开始按钮_点击操作
功能 开始_点击()
    ret=""
    temp=""
    i=0
    // 在注册表中遍历 ethernet内容
    while(ret!="ethernet")
        if(i<10)
            temp=0&i
            i++
        else
            temp=i
            i++
        endif
        paht="HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\00"&temp&"\\Ndi\\Interfaces"
        // 获取注册表里 paht项的值
        ret=系统.注册表获取键值(paht,"LowerRange")
    endwhile
    // 拼接IPpath字符串
    IPpath="HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\00"&temp&"\\Linkage"
    // 获取IPpath项的值
    ipcode=系统.注册表获取键值(IPpath,"RootDevice")
    // 获取注册表中本机的IP地址值
    ret=系统.注册表获取键值("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Interfaces\\"&ipcode,"IPAddress")
    // 使用消息框命令显示出本机的IP值
    辅助.消息框(ret,"通过注册表获取")
功能结束

//退出按钮_点击操作
功能 退出_点击()
    // 调用自定义方法
    sru=获取本机IP.外网的IP("http://iframe.ip138.com/ic.asp")
    // 查找 [ 字符在字符串中的起始位置
    start=str.findstr(sru,"[")
    // 查找 ] 字符在字符串中的起始位置
    end=str.findstr(sru,"]")
    // 通过字符串截取命令截取[ ] 直接的IP地址内容,并以消息框模式显示出来。
    help.messagebox(str.strsub(sru,start+1,end),"通过系统对象xmlhttp获取")    
功能结束

空间结束
//空间结束语句