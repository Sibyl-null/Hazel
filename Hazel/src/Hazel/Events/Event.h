#pragma once
#include "hzpch.h"
#include "Hazel/Core.h"

namespace Hazel {
	// 当前事件是阻塞的，这意味着当一个事件发生时立即被分派，必须立即处理。
	// 将来，更好的策略可能是在缓冲事件，在Update阶段的“事件”部分处理它们。

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// 事件分组（事件组由多个EventType具体事件组成）
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication		= BIT(0),
		EventCategoryInput				= BIT(1),
		EventCategoryKeyboard			= BIT(2),
		EventCategoryMouse				= BIT(3),
		EventCategoryMouseButton		= BIT(4)
	};

// 使用Event基类指针时，调用GetEventType函数可以得到原本的EventType（多态）
// ## 将两个标识符或预处理器符号连接在一起，形成一个新的标识符
// # 用于将宏参数转化为字符串字面量，自动添加双引号
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define Event_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// base class
	class HAZEL_API Event {
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;     // 事件类别
		virtual int GetCategoryFlags() const = 0;		// 所属事件组
		virtual const char* GetName() const = 0;		// 事件名，调试用
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher {
		// 接受类型为 T& 的参数并返回 bool 值
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event){}

		template<typename T>
		bool Dispath(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				// (T*)&m_Event：将 m_Event 的地址强制转换为类型 T* 的指针
				// *(T*)&m_Event：将上述指针进行解引用，得到类型为 T 的对象
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e){
		return os << e.ToString();
	}
}