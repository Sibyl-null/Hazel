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

**Event** 为事件抽象类，由 **EventDispatcher** 类进行调度。
```
CustomEvent e;    // 初始化事件
EventDispatcher dispatcher(e);   // 创建事件调度器
dispatcher.Dispath<CustomEvent>(OnCustomEvent);    // 进行事件调度

// 具体调度逻辑
bool OnCustomEvent(CustomEvent& event){
   [...]
}
```

### 引擎层级
可根据引擎不同功能模块划分为不同层级，具体层级实现可由 Sandbox 自定义。

**Layer** 是层级基类，具有以下生命周期函数：
```
virtual void OnAttach() {}             // 层级附加
virtual void OnDetach() {}             // 层级分离
virtual void OnUpdate() {}             // 层级更新
virtual void OnEvent(Event& event) {}  // 层级事件响应
```

**LayerStack** 类是存储 Layer 的集合类，可以添加移除 Layer。内部使用 vector 实现。
普通添加层级时，会置于最底层。使用 Overlay 添加层级时，会至于最顶层。

Application 中持有 LayerStack 实例，处理各个层级的 Update 和 OnEvent。
* Update 执行顺序：  底层 -> 顶层，全部执行。
* OnEvent 执行顺序： 顶层 -> 底层。若某层成功处理事件，则中断传递。