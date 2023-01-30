#pragma once

#include "Shader.hpp"
#include "Types.hpp"
#include "Buffer.hpp"

struct SampleDesc
{
	uint32 count;
	uint32 quality;
};

struct LayoutElement
{
	const char* HLSLSemantics;
	int32 inputIndex = 0;
	EImageFormat format = EImageFormat::UNKNOWN;
	int32 slot = 0;
	size_t offset = 0ull;

	enum Classification 
	{
		PerVertex, PerInstance
	} classification = PerVertex;
};

struct InputLayout
{
	int32 numLayoutElements = 0;
	LayoutElement* layoutElements = nullptr;
};

enum ColorWriteEnable
{
	ColorWriteEnable_Red   = 1,
	ColorWriteEnable_Green = 2,
	ColorWriteEnable_Blue  = 4,
	ColorWriteEnable_Alpha = 8,
	ColorWriteEnable_All = 1 | 2 | 4 | 8,
};

// Incomplete pipeline stage enums.
enum class EPipelineStage : uint32
{
	None = 0,
	VertexInput = 1 << 0,
	VertexShader = 1 << 1,
	PixelShader = 1 << 2,
	EarlyPixelTests = 1 << 3,
	LatePixelTests = 1 << 4,
	RenderTarget = 1 << 5,
	ComputeShader = 1 << 6,
	Transfer = 1 << 7,
	AllCommands = 1 << 8,
	Host = 1 << 20 // API exclusive pipeline enum
};
ENUM_FLAGS(EPipelineStage, uint32);

enum class EPipelineAccess : uint32
{
	None = 0,
	VertexRead = 1 << 0,
	IndexRead = 1 << 1,
	ShaderRead = 1 << 2,
	ShaderWrite = 1 << 3,
	RenderTargetRead = 1 << 4,
	RenderTargetWrite = 1 << 5,
	DepthStencilRead = 1 << 6,
	DepthStencilWrite = 1 << 7,
	TransferRead = 1 << 8,
	TransferWrite = 1 << 9,
	MemoryRead = 1 << 10,
	MemoryWrite = 1 << 11,
	// API exclusive pipeline enums
	HostRead = 1 << 20,
	HostWrite = 1 << 21
};
ENUM_FLAGS(EPipelineAccess, uint32);

enum class EDescriptorType : uint32
{
	Unknown = -1,
	Sampler,
	ShaderResource,
	ConstantBuffer,
	UnorderedAccessImage,
	UnorderedAccessBuffer,
	PushConstant,
	Count
};

struct Sampler
{ };

struct IImage 
{ };

struct DescriptorBindingDesc
{
	int BindingID = -1;
	EDescriptorType Type = EDescriptorType::Unknown;
	EShaderType TargetShader = EShaderType::None;
	uint32 ArraySize = -1;

	union
	{
		Sampler* sampler = nullptr;
		IBuffer* buffer;
		IImage* image;
	};
};

struct DescriptorSetDesc
{
	uint32 m_BindingCount = 0u;
	DescriptorBindingDesc m_pBindings[16];
};

struct PipelineBarrier
{
	EResourceUsage m_CurrentUsage = EResourceUsage::Unknown;
	EPipelineStage m_CurrentStage = EPipelineStage::None;
	EPipelineAccess m_CurrentAccess = EPipelineAccess::None;
	EResourceUsage m_NextUsage = EResourceUsage::Unknown;
	EPipelineStage m_NextStage = EPipelineStage::None;
	EPipelineAccess m_NextAccess = EPipelineAccess::None;
};

struct BlendDesc
{
	bool BlendEnable;

	EBlendFactor SrcBlend;
	EBlendFactor DestBlend;
	EBlendOp BlendOp;
	EBlendFactor SrcBlendAlpha;
	EBlendFactor DestBlendAlpha;
	EBlendOp BlendOpAlpha;
	ColorWriteEnable RenderTargetWriteMask = ColorWriteEnable_All;
};

struct InputLayout
{
	Slice<const char> name{};// string view
	VertexAttribType Type = VertexAttribType::None;
};

struct PipelineInfo
{
	static constexpr int MaxRenderTargets = 16;

	IShader* VertexShader      = nullptr;
	IShader* FragmentShader    = nullptr;
	IShader* HullShader        = nullptr;
	IShader* DomainShader      = nullptr;
	IShader* GeometryShader    = nullptr;
	IShader* TesellationShader = nullptr;

	EPrimitiveType primitiveType = EPrimitiveType::Triangle;
	ECullMode cullMode = ECullMode::None;
	EFillMode fillMode = EFillMode::Fill;
	bool counterClockWise = false;

	int32 numRenderTargets = 0;
	EImageFormat DepthStencilFormat = EImageFormat::UNKNOWN;
	EImageFormat RTVFormats[MaxRenderTargets] = {};

	bool AlphaToCoverageEnable = false;
	bool IndependentBlendEnable = false;

	BlendDesc RenderTargetBlendDescs[MaxRenderTargets] = {};
	
	int32 descriptorSetCount = 0;
	DescriptorSetDesc descriptorSet;
	
	int32 numInputLayout = 0;
	InputLayout inputLayouts[8] = {};

	bool enableMultiSampling = false;
	SampleDesc sampleDesc = {}; // multisampling parameters
};

struct IPipeline
{
	PipelineInfo info;
};