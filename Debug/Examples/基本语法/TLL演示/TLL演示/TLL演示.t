空间 TLL演示
//默认空间
//调用TLL加函数,实现两个变量加法运算
功能 按钮0_点击()//按钮0点击事件功能函数
加数=编辑框.获得文本("编辑框0")//获得界面控件"编辑框0"里的文本,并且赋值给变量"加数"    
被加数=编辑框.获得文本("编辑框1") //获得界面控件"编辑框1"里的文本,并且赋值给变量"被加数"    
结果=TLL示例.加(加数,被加数)//调用TLL文件"TLL示例"里的"加"函数,把两个变量作为参数传入函数,并且把返回值赋值给变量"结果"
辅助.消息框(结果)//用消息框命令输出变量"结果"里的值
功能结束

功能 按钮1_点击()//按钮1点击事件功能函数
减数=编辑框.获得文本("编辑框0")//获得界面控件"编辑框0"里的文本,并且赋值给变量"减数"        
被减数=编辑框.获得文本("编辑框1") //获得界面控件"编辑框1"里的文本,并且赋值给变量"被减数"        
结果=TLL示例.减(减数,被减数)//调用TLL文件"TLL示例"里的"减"函数,把两个变量作为参数传入函数,并且把返回值赋值给变量"结果"
辅助.消息框(结果)//用消息框命令输出变量"结果"里的值
功能结束

功能 按钮2_点击()//按钮2点击事件功能函数
乘数=编辑框.获得文本("编辑框0")    //获得界面控件"编辑框0"里的文本,并且赋值给变量"乘数"      
被乘数=编辑框.获得文本("编辑框1") //获得界面控件"编辑框1"里的文本,并且赋值给变量"被乘数"         
结果=TLL示例.乘(乘数,被乘数)//调用TLL文件"TLL示例"里的"乘"函数,把两个变量作为参数传入函数,并且把返回值赋值给变量"结果"
辅助.消息框(结果)//用消息框命令输出变量"结果"里的值



功能结束

功能 按钮3_点击()//按钮2点击事件功能函数
除数=编辑框.获得文本("编辑框0")    //获得界面控件"编辑框0"里的文本,并且赋值给变量"除数"      
被除数=编辑框.获得文本("编辑框1") //获得界面控件"编辑框1"里的文本,并且赋值给变量"被除数"            
结果=TLL示例.除(除数,被除数)//调用TLL文件"TLL示例"里的"除"函数,把两个变量作为参数传入函数,并且把返回值赋值给变量"结果"
辅助.消息框(结果)//用消息框命令输出变量"结果"里的值
功能结束

//**********************************************************************************************************************************************
//TLL的调用与跨空间调用一样
//好处是不需要写预定义语句,保护源码的私密性
//***********************************************************************************************************************************************
空间结束
//空间结束语句