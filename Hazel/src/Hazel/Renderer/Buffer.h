#pragma once


namespace Hazel
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, 
		Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case Hazel::ShaderDataType::Float:		return 4;
			case Hazel::ShaderDataType::Float2:		return 4 * 2;
			case Hazel::ShaderDataType::Float3:		return 4 * 3;
			case Hazel::ShaderDataType::Float4:		return 4 * 4;
			case Hazel::ShaderDataType::Mat3:		return 4 * 3 * 3;
			case Hazel::ShaderDataType::Mat4:		return 4 * 4 * 4;
			case Hazel::ShaderDataType::Int:		return 4;
			case Hazel::ShaderDataType::Int2:		return 4 * 2;
			case Hazel::ShaderDataType::Int3:		return 4 * 3;
			case Hazel::ShaderDataType::Int4:		return 4 * 4;
			case Hazel::ShaderDataType::Bool:		return 1;
		}
		HZ_CORE_ASSERT(false, "Unknow ShaderDataType!");
		return 0;
	}

	struct BufferElements
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;

		BufferElements(const std::string& name, ShaderDataType type)
			:Name(name), Type(type), Size(0), Offset(0)
		{

		}
	};

	class BufferLayout
	{
	public:
		BufferLayout();

		inline const std::vector<BufferElements>& GetElements() const { return m_Elements; }

	private:
		std::vector<BufferElements> m_Elements;
	};

	class VertexBuffer
	{
	public:
		~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);

	};

}
