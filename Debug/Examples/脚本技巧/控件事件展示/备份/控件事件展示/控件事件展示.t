空间 控件事件展示
//默认空间

//开始按钮_点击操作
功能 开始_点击()
    
功能结束

//退出按钮_点击操作
功能 退出_点击()
    
    辅助.退出()
功能结束


功能 标签0_点击()
    //这里添加你要执行的代码
    // 这里是点击标签的时候触发的事件
    辅助.消息框("我是标签的点击事件！你明白了吗？")
功能结束


功能 复选框0_点击()
    //这里添加你要执行的代码
    // 这里是点击复选框的时候触发的事件
    如果(复选框.获得状态("复选框0"))
        辅助.消息框("选中")
    否则
        辅助.消息框("取消选中")
    如果结束
功能结束


功能 列表框0_选择改变()
    //这里添加你要执行的代码
    // 这里是改变列表框选项的时候触发的事件
    内容 = 列表框.得到选中文本("列表框0")
    辅助.消息框("你选中的内容是:"&内容)
功能结束


功能 滑块0_鼠标释放()
    //这里添加你要执行的代码
    // 这里是改变滑块的位置时触发的事件
    位置 = 滑块.得到位置("滑块0")
    // 设置进度条的位置
    进度条.设置进度("进度条0",位置)
功能结束


功能 下拉框0_选择改变()
    //这里添加你要执行的代码
    // 这里是改变下拉框选项的时候触发的事件
    选项 = 下拉框.得到选项("下拉框0")    // 获取下拉框的选中选项
    内容 = 下拉框.获得内容("下拉框0",选项)  // 获取选择的内容
    辅助.消息框(内容,"你选择的内容")
功能结束

空间结束
//空间结束语句