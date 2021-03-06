空间 一键百度搜索
//默认空间

// 自定义功能,用于操作浏览器控件线程
功能 搜索线程()
    // 跳转到百度搜索界面
    网页.转到("浏览器0","www.baidu.com")
    循环(真)
        // 判断网页页面是否加载完成,对网页元素操作,请确保网页加载完成！否则会出异常错误！
        如果(网页.加载完成("浏览器0"))
            // 如果加载完成则退出循环
            跳出
        如果结束
    循环结束    
    // 在搜索输入框里输入TC简单开发
    网页.输入元素文本("浏览器0","TC简单开发","id:kw")
    // 点击百度一下按钮
    网页.点击元素("浏览器0","id:su") 
    辅助.消息框("成功完成搜索操作！","恭喜你")
功能结束

//开始按钮_点击操作
功能 开始_点击()
    // 因为我们的程序里有循环判断网页状态代码，
    // 因此这里使用线程模式操作浏览器控件，以避免界面出现卡死现象。
    线程.开启("一键百度搜索.搜索线程","")
功能结束

//退出按钮_点击操作
功能 退出_点击()

辅助.退出()
功能结束

空间结束
//空间结束语句