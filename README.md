# Hazel
## 注意项
* vs 运行程序前，修改 GLFW 工程配置。C/C++ -> 代码生成 -> 运行库：多线程(/MT)
  
## 代码解析
主要分为两个工程，Hazel 和 Sandbox。Sandbox 为启动项目。
Hazel 是引擎核心代码（dll），Sandbox 是客户端（exe），Sandbox 依赖 Hazel。

### EntryPoint
程序入口的 main 方法作为 .h 文件包含在 Hazel 工程里，由 Sandbox 里的 .cpp 文件 include 进行调用。 
* Hazel 工程部分：
   * 定义 Application 基类，声明 CreateApplication 全局方法。
   * 在 EntryPoint.h 文件中定义 main 函数，使用外部定义的 CreateApplication 函数（extern）创建 Application 对象，运行引擎循环。
  
* Sandbox 工程部分：
   * 定义继承自 Application 的子类 Sandbox。
   * 定义 CreateApplication 函数实现，返沪 Sandbox 对象。

### 事件系统
一个事件具有事件类型和其所属的事件组，例如键盘事件组包含按键按下和按键抬起两个事件类型。

Event 为事件抽象类，由 EventDispatcher 类进行分发调用。