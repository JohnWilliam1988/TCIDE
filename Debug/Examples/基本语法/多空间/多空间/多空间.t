#include"子空间一.t"
空间 多空间
//默认空间
//***************************跨空间调用函数***********************************
功能 按钮0_点击()//按钮0点击事件功能函数
    test.spacetest()
    //跨空间调用函数的方法,子空间名.函数名(参数1,参数2,....),函数无参数可以省略
功能结束
//**************************************************************
//*************************跨空间调用全局变量***********************************
功能 按钮1_点击()//按钮1点击事件功能函数
    辅助.消息框(子空间_A.我是全局变量)
    //跨空间调用全局变量方法,子空间名.变量名
功能结束
//**************************************************************
//*************************跨空间赋值全局变量***********************************
功能 按钮2_点击()//按钮2点击事件功能函数
    //跨空间给全局变量赋值
    子空间_A.我是全局变量="TC你好!"
    //跨空间调用全局变量方法,子空间名.变量名
    辅助.消息框(子空间_A.我是全局变量)
功能结束
//**************************************************************
空间结束
//空间结束语句
//***************************自定义子空间***********************************
//定义子空间,空间名必须符合TC命名规则
//首字必须是字母或者汉字,只能由字母,汉字,数字和下划线组成
空间 test
功能 spacetest()//自定义函数,调用该函数就会执行函数内的命令,
    辅助.消息框("调用了test空间的spacetest函数")
    //TC工具封装的辅助空间里的消息框函数
功能结束
空间结束