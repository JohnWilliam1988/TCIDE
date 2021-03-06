空间 函数调用
//默认空间
//*************************************普通传参函数*********************************************
//自定义一个函数,给函数参数设置默认值,就是在参数里给变量赋值,
功能 普通传参(变量1,变量2="默认参数")//自定义函数方法,功能+空格+函数名(参数1,参数2,.......)//参数可以省略
辅助.消息框("传入了参数1:"&变量1&",传入了参数2:"&变量2)//使用消息框输出变量1与变量2的值
//在函数调用的参数中,&为字符串连接符
功能结束//每一个功能对应一个功能结束语句
//***********************************************************************************************
//***********************************普通传参函数调用*******************************************
功能 按钮0_点击()//按钮的点击事件功能函数
函数调用.普通传参("我是参数1")////函数的调用方法,空间名.函数名(参数1,参数2,......),只传入一个参数,第二个参数就为默认参数值
函数调用.普通传参("我是参数1","我是参数2")//传入两个参数
功能结束//每一个功能对应一个功能结束语句
//***********************************************************************************************
//*************************************引用传参函数*********************************************
功能 引用传参(&变量1,&变量2=0)//在自定义函数参数中,&为引用传值符
变量1="我改变了参数1"//给传入的变量1重新赋值
变量2="我改变了参数2" //给传入的变量2重新赋值
功能结束 //每一个功能对应一个功能结束语句 
//***********************************************************************************************
//***********************************引用传参函数调用*******************************************
功能 按钮1_点击()//按钮的点击事件功能函数
变量1="我是参数1",变量2="我是参数2"//给变量1,变量2赋值
辅助.消息框(变量1&","&变量2)//用消息框输出变量1,变量2查看,  
函数调用.引用传参(变量1)//把变量1传入"引用传参"函数
辅助.消息框(变量1&","&变量2)//用消息框输出变量1,变量2查看,效果:变量1被重新赋值了,而变量2却没有  
函数调用.引用传参(变量1,变量2)//把变量1和变量2传入"引用传参"函数:效果变量1和变量2都被重新赋值了
辅助.消息框(变量1&","&变量2)//用消息框输出变量1,变量2查看  
功能结束//每一个功能对应一个功能结束语句
//***********************************************************************************************
//*************************************无返回值函数*********************************************
功能 无返回值函数()//自定义函数方法,功能+空格+函数名(参数1,参数2,.......)//参数可以省略
函数调用.被调用的函数()//在函数内调用其他的函数    
功能结束
功能 被调用的函数()//自定义函数
 辅助.消息框("你调用了该函数")//调用函数则会执行函数体内的命令  
功能结束//每一个功能对应一个功能结束语句
//***********************************************************************************************
//***********************************无返回值函数调用*******************************************
功能 按钮3_点击()//按钮的点击事件功能函数
函数调用.无返回值函数()//函数的调用方法,空间名.函数名()
功能结束//每一个功能对应一个功能结束语句
//***********************************************************************************************
//*************************************带返回值函数*********************************************
功能 带返回值函数(内容1,内容2)//自定义普通传参函数
如果(内容1>内容2)//比较传入的参数1是否大于参数2
    返回 "A大于B"//如果参数1大于参数2,则设置函数返回值为"A大于B"
否则//如果参数1不大于参数2,则执行否则后面的代码
    如果(内容1<内容2)//比较传入的参数1是否小于参数2
        返回 "A小于B"//如果参数1小于参数2,则设置函数返回值为"A小于B"
    否则//如果参数1不小于参数2,则执行否则后面的代码
        返回 "A等于B"//设置函数返回值为"A等于B"
    如果结束//每一个如果对应一个"如果结束"
如果结束//每一个如果对应一个"如果结束"
功能结束//每一个功能对应一个功能结束语句
//通过设置不同的函数返回值,我们就可以在调用该函数的时候通过获得返回值来了解函数的执行情况
//***********************************************************************************************
//***********************************无返回值函数调用*******************************************
功能 按钮2_点击()//按钮的点击事件功能函数
A=编辑框.获得文本("编辑框0")//获得编辑框0的文本,赋值给变量 "A"
B=编辑框.获得文本("编辑框1")//获得编辑框1的文本,赋值给变量 "B"
函数的返回值=函数调用.带返回值函数(A,B)//调用函数"带返回值的函数",把"A","B"两个变量的值传入函数,并把函数的返回值赋值给变量"函数的返回值"
辅助.消息框(函数的返回值)//用消息框输出变量"函数的返回值"里的值
//通过函数的返回值我们可以了解函数的执行情况
功能结束//每一个功能对应一个功能结束语句
空间结束
//空间结束语句