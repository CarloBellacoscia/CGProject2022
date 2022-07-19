#include "MyProject.hpp"

// Load all the paths needed 
const std::string MAP_PATH = "textures/map.txt";

const std::string KEY_MODEL_PATH = "models/key.obj";
const std::string GKEY_TEXTURE_PATH = "textures/GoldKey.png";
const std::string CKEY_TEXTURE_PATH = "textures/CopperKey.png";

const std::string FLOOR_MODEL_PATH = "models/floor.obj";
const std::string FLOOR_TEXTURE_PATH = "textures/EOBSLM04.png";

const std::string LEVER_MODEL_PATH = "models/lever_test.obj";
const std::string LEVER_TEXTURE_PATH = "textures/lever.png";

const std::string DOOR_MODEL_PATH = "models/door.obj";
const std::string DOOR_TEXTURE_PATH = "textures/EBBRD01.png";

const std::string SINGLE_DOOR_MODEL_PATH = "models/singleDoor.obj";
const std::string SINGLE_DOOR_TEXTURE_PATH = "textures/EOBDR02.png";

const std::string CEILING_MODEL_PATH = "models/ceiling.obj";
const std::string CEILING_TEXTURE_PATH = "textures/EOBFLT12.png";

const std::string WALLE_MODEL_PATH = "models/wallE.obj";
const std::string WALLN_MODEL_PATH = "models/wallN.obj";
const std::string WALLS_MODEL_PATH = "models/wallS.obj";
const std::string WALLW_MODEL_PATH = "models/wallW.obj";
const std::string WALL_TEXTURE_PATH = "textures/EOBRB01.png";

const std::string TEXT_MODEL_PATH = "models/text.obj";
const std::string INTERACT_TEXTURE_PATH = "textures/interact.png";
const std::string NEED_KEY_TEXTURE_PATH = "textures/needKey.png";

// Initialize some useful data
glm::vec3 ang = glm::vec3(10.0f, 90.0f, 0.0f);
glm::vec3 pos = glm::vec3(0.85797f, -0.75f, -2.81876f);

int MAP[24][24];

float leverAngle[3] = { 0.0f, 0.0f, 0.0f };
int doorStatus[5] = {0,0,0,0,0};
float deltaDoor[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
float deltaKey[2] = { 0.0f, 0.0f };
int hasKey[2] = { 0, 0 };

float deltaInteract[5] = { -10.0f, -10.0f, -10.0f, -10.0f, -10.0f };
float deltaNeedKey[2] = { -10.0f, -10.0f };



// The uniform buffer object used in this example
struct globalUniformBufferObject {
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
};


// MAIN ! 
class MyProject : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSLglobal;
	DescriptorSetLayout DSLobj;

	// Pipelines [Shader couples]
	Pipeline P1;

	
	Model M_Key;
	Texture T_GKey;
	DescriptorSet DS_GKey;	// instance DSLobj
	Texture T_CKey;
	DescriptorSet DS_CKey;	// instance DSLobj

	Model M_Floor;
	Texture T_Floor;
	DescriptorSet DS_Floor;	// instance DSLobj

	Model M_Lever;
	Texture T_Lever;
	DescriptorSet DS_Lever1;	// instance DSLobj
	DescriptorSet DS_Lever2;	// instance DSLobj
	DescriptorSet DS_Lever3;	// instance DSLobj

	Model M_Door;
	Texture T_Door;
	DescriptorSet DS_Door1;	// instance DSLobj
	DescriptorSet DS_Door2;	// instance DSLobj
	DescriptorSet DS_Door3;	// instance DSLobj
	DescriptorSet DS_Door4;	// instance DSLobj
	DescriptorSet DS_Door5;	// instance DSLobj

	Model M_SDoor;
	Texture T_SDoor;
	DescriptorSet DS_SDoor1;	// instance DSLobj
	DescriptorSet DS_SDoor2;	// instance DSLobj
	DescriptorSet DS_SDoor3;	// instance DSLobj
	DescriptorSet DS_SDoor4;	// instance DSLobj
	DescriptorSet DS_SDoor5;	// instance DSLobj

	Model M_Ceiling;
	Texture T_Ceiling;
	DescriptorSet DS_Ceiling;	// instance DSLobj

	Texture T_Wall; 
	Model M_WallE;
	DescriptorSet DS_WallE;	// instance DSLobj
	Model M_WallN;
	DescriptorSet DS_WallN;	// instance DSLobj
	Model M_WallS;
	DescriptorSet DS_WallS;	// instance DSLobj
	Model M_WallW;
	DescriptorSet DS_WallW;	// instance DSLobj

	Model M_Text;
	Texture T_Interact;
	Texture T_NeedKey;
	DescriptorSet DS_Interact1;	// instance DSLobj
	DescriptorSet DS_Interact2;	// instance DSLobj
	DescriptorSet DS_Interact3;	// instance DSLobj
	DescriptorSet DS_Interact4;	// instance DSLobj
	DescriptorSet DS_Interact5;	// instance DSLobj
	DescriptorSet DS_NeedKey1;	// instance DSLobj
	DescriptorSet DS_NeedKey2;	// instance DSLobj

	DescriptorSet DS_global;	// instance DSLglobal
	
	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 1500;
		windowHeight = 750;
		windowTitle = "Dungeon";
		initialBackgroundColor = {0.01f, 0.03f, 0.01f, 1.0f};
		
		// Descriptor pool sizes (CHANGE BASED ON THE NUMEBRE OF THE OBJECTS)
		uniformBlocksInPool = 31;	//should be 11
		texturesInPool = 31;		//shoud be 10
		setsInPool = 31;
	}
	
	// Here you load and setup all your Vulkan objects
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]
		DSLobj.init(this, {
					// this array contains the binding:
					// first  element : the binding number
					// second element : the time of element (buffer or texture)
					// third  element : the pipeline stage where it will be used
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
				  });
		DSLglobal.init(this, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
			});

		setMap();

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, "shaders/vert.spv", "shaders/frag.spv", {&DSLglobal, &DSLobj});

		// Models, textures and Descriptors (values assigned to the uniforms)
		M_Key.init(this, KEY_MODEL_PATH);
		T_GKey.init(this, GKEY_TEXTURE_PATH);
		DS_GKey.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_GKey}
			});
		T_CKey.init(this, CKEY_TEXTURE_PATH);
		DS_CKey.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_CKey}
			});

		M_Floor.init(this, FLOOR_MODEL_PATH);
		T_Floor.init(this, FLOOR_TEXTURE_PATH);
		DS_Floor.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Floor}
			});

		M_Lever.init(this, LEVER_MODEL_PATH);
		T_Lever.init(this, LEVER_TEXTURE_PATH);
		DS_Lever1.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Lever}
			});
		DS_Lever2.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Lever}
			});
		DS_Lever3.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Lever}
			});

		M_Door.init(this, DOOR_MODEL_PATH);
		T_Door.init(this, DOOR_TEXTURE_PATH);
		DS_Door1.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Door}
			});
		DS_Door2.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Door}
			});
		DS_Door3.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Door}
			});
		DS_Door4.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Door}
			});
		DS_Door5.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Door}
			});

		M_SDoor.init(this, SINGLE_DOOR_MODEL_PATH);
		T_SDoor.init(this, SINGLE_DOOR_TEXTURE_PATH);
		DS_SDoor1.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_SDoor}
			});
		DS_SDoor2.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_SDoor}
			});
		DS_SDoor3.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_SDoor}
			});
		DS_SDoor4.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_SDoor}
			});
		DS_SDoor5.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_SDoor}
			});

		M_Ceiling.init(this, CEILING_MODEL_PATH);
		T_Ceiling.init(this, CEILING_TEXTURE_PATH);
		DS_Ceiling.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Ceiling}
			});

		T_Wall.init(this, WALL_TEXTURE_PATH);
		M_WallE.init(this, WALLE_MODEL_PATH);
		DS_WallE.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Wall}
			});
		M_WallN.init(this, WALLN_MODEL_PATH);
		DS_WallN.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Wall}
			});
		M_WallS.init(this, WALLS_MODEL_PATH);
		DS_WallS.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Wall}
			});
		M_WallW.init(this, WALLW_MODEL_PATH);
		DS_WallW.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Wall}
			});

		M_Text.init(this, TEXT_MODEL_PATH);
		T_Interact.init(this, INTERACT_TEXTURE_PATH);
		T_NeedKey.init(this, NEED_KEY_TEXTURE_PATH);

		DS_Interact1.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Interact}
			});
		DS_Interact2.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Interact}
			});
		DS_Interact3.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Interact}
			});
		DS_Interact4.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Interact}
			});
		DS_Interact5.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Interact}
			});

		DS_NeedKey1.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_NeedKey}
			});
		DS_NeedKey2.init(this, &DSLobj, {
		{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
		{1, TEXTURE, 0, &T_NeedKey}
			});
		


		DS_global.init(this, &DSLglobal, {
				{0, UNIFORM, sizeof(globalUniformBufferObject), nullptr}
			});
	}

	// Here you destroy all the objects you created!		
	void localCleanup() {

		DS_GKey.cleanup();
		T_GKey.cleanup();
		DS_CKey.cleanup();
		T_CKey.cleanup();
		M_Key.cleanup();

		DS_Floor.cleanup();
		T_Floor.cleanup();
		M_Floor.cleanup();

		DS_Lever1.cleanup();
		DS_Lever2.cleanup();
		DS_Lever3.cleanup();
		T_Lever.cleanup();
		M_Lever.cleanup();

		DS_Door1.cleanup();
		DS_Door2.cleanup();
		DS_Door3.cleanup();
		DS_Door4.cleanup();
		DS_Door5.cleanup();
		T_Door.cleanup();
		M_Door.cleanup();

		DS_SDoor1.cleanup();
		DS_SDoor2.cleanup();
		DS_SDoor3.cleanup();
		DS_SDoor4.cleanup();
		DS_SDoor5.cleanup();
		T_SDoor.cleanup();
		M_SDoor.cleanup();

		DS_Ceiling.cleanup();
		T_Ceiling.cleanup();
		M_Ceiling.cleanup();

		DS_WallE.cleanup();
		M_WallE.cleanup();
		DS_WallN.cleanup();
		M_WallN.cleanup();
		DS_WallS.cleanup();
		M_WallS.cleanup();
		DS_WallW.cleanup();
		M_WallW.cleanup();
		T_Wall.cleanup();

		DS_Interact1.cleanup();
		DS_Interact2.cleanup();
		DS_Interact3.cleanup();
		DS_Interact4.cleanup();
		DS_Interact5.cleanup();
		DS_NeedKey1.cleanup();
		DS_NeedKey2.cleanup();
		T_Interact.cleanup();
		T_NeedKey.cleanup();
		M_Text.cleanup();

		DS_global.cleanup();

		P1.cleanup();

		DSLglobal.cleanup();
		DSLobj.cleanup();
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
				
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				P1.graphicsPipeline);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 0, 1, &DS_global.descriptorSets[currentImage],
			0, nullptr);


		
		VkBuffer vertexBuffers[] = { M_Key.vertexBuffer};
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, M_Key.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_GKey.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Key.indices.size()), 1, 0, 0, 0);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_CKey.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Key.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffers2[] = { M_Floor.vertexBuffer };
		VkDeviceSize offsets2[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
		vkCmdBindIndexBuffer(commandBuffer, M_Floor.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Floor.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Floor.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffers3[] = { M_Lever.vertexBuffer };
		VkDeviceSize offsets3[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers3, offsets3);
		vkCmdBindIndexBuffer(commandBuffer, M_Lever.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Lever1.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Lever.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Lever2.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Lever.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Lever3.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Lever.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffers4[] = { M_Door.vertexBuffer };
		VkDeviceSize offsets4[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers4, offsets4);
		vkCmdBindIndexBuffer(commandBuffer, M_Door.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Door1.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Door.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Door2.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Door.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Door3.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Door.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Door4.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Door.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Door5.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Door.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffer5[] = { M_SDoor.vertexBuffer };
		VkDeviceSize offsets5[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffer5, offsets5);
		vkCmdBindIndexBuffer(commandBuffer, M_SDoor.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_SDoor1.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_SDoor.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_SDoor2.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_SDoor.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_SDoor3.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_SDoor.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_SDoor4.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_SDoor.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_SDoor5.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_SDoor.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffer6[] = { M_Ceiling.vertexBuffer };
		VkDeviceSize offsets6[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffer6, offsets6);
		vkCmdBindIndexBuffer(commandBuffer, M_Ceiling.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Ceiling.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_Ceiling.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffer7[] = { M_WallE.vertexBuffer };
		VkDeviceSize offsets7[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffer7, offsets7);
		vkCmdBindIndexBuffer(commandBuffer, M_WallE.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_WallE.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallE.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffer8[] = { M_WallN.vertexBuffer };
		VkDeviceSize offsets8[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffer8, offsets8);
		vkCmdBindIndexBuffer(commandBuffer, M_WallN.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_WallN.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallN.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffer9[] = { M_WallS.vertexBuffer };
		VkDeviceSize offsets9[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffer9, offsets9);
		vkCmdBindIndexBuffer(commandBuffer, M_WallS.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_WallS.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallS.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffer10[] = { M_WallW.vertexBuffer };
		VkDeviceSize offsets10[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffer10, offsets10);
		vkCmdBindIndexBuffer(commandBuffer, M_WallW.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_WallW.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallW.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffer11[] = { M_Text.vertexBuffer };
		VkDeviceSize offsets11[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffer11, offsets11);
		vkCmdBindIndexBuffer(commandBuffer, M_Text.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Interact1.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallW.indices.size()), 1, 0, 0, 0);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Interact2.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallW.indices.size()), 1, 0, 0, 0);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Interact3.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallW.indices.size()), 1, 0, 0, 0);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Interact4.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallW.indices.size()), 1, 0, 0, 0);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_Interact5.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallW.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_NeedKey1.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallW.indices.size()), 1, 0, 0, 0);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_NeedKey2.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_WallW.indices.size()), 1, 0, 0, 0);
		
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {

		static auto startTime = std::chrono::high_resolution_clock::now();
		static float lastTime = 0.0f;
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>
					(currentTime - startTime).count();


		float deltaT = time - lastTime;

		lastTime = time;

		static float debounce = time;

		glm::vec3 lastPos = pos;

		const float margin = 0.2;
		const float speed = 0.85;
		const float angSpeed = 50.0;

		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			ang.y -= deltaT * angSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			ang.y += deltaT * angSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_UP)) {
			ang.x -= deltaT * angSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN)) {
			ang.x += deltaT * angSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_R)) {
			ang = glm::vec3(10.0f, 90.0f, 0.0f);
			pos = glm::vec3(0.85797f, -0.75f, -2.81876f);
		}

		if (glfwGetKey(window, GLFW_KEY_W)) {
			lastPos = pos;
			lastPos += speed * glm::vec3(glm::rotate(glm::mat4(1.0f), -glm::radians(ang.y),
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0, 0, 1, 1)) * deltaT;
			if (possiblePos(lastPos, margin) && possiblePos(lastPos, -margin)) {
				pos = lastPos;
			}

		}

		if (glfwGetKey(window, GLFW_KEY_S)) {
			lastPos = pos;
			lastPos -= speed * glm::vec3(glm::rotate(glm::mat4(1.0f), -glm::radians(ang.y),
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0, 0, 1, 1)) * deltaT;
			if (possiblePos(lastPos, margin) && possiblePos(lastPos, -margin)) {
				pos = lastPos;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_D)) {
			lastPos = pos;
			lastPos -= speed * glm::vec3(glm::rotate(glm::mat4(1.0f), -glm::radians(ang.y),
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1, 0, 0, 1)) * deltaT;
			if (possiblePos(lastPos, margin) && possiblePos(lastPos, -margin)) {
				pos = lastPos;
			}

		}

		if (glfwGetKey(window, GLFW_KEY_A)) {
			lastPos = pos;
			lastPos += speed * glm::vec3(glm::rotate(glm::mat4(1.0f), -glm::radians(ang.y),
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1, 0, 0, 1)) * deltaT;
			if (possiblePos(lastPos, margin) && possiblePos(lastPos, -margin)) {
				pos = lastPos;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			if (time - debounce > 0.33) {
				interact();
				debounce = time;
			}
		}

					
		globalUniformBufferObject gubo{}; 
		UniformBufferObject ubo{};

		void* data;

		// View from the top
		/*gubo.view = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f),
							   glm::vec3(3.0f, 0.0f, 0.0f),
							   glm::vec3(0.0f, 1.0f, 0.0f));*/

		// First person view
		gubo.view = glm::rotate(glm::mat4(1.0), glm::radians(ang.z), glm::vec3(0, 0, 1)) *
					glm::rotate(glm::mat4(1.0), glm::radians(ang.x), glm::vec3(1, 0, 0)) *
					glm::rotate(glm::mat4(1.0), glm::radians(ang.y), glm::vec3(0, 1, 0)) *
					glm::translate(glm::mat4(1.0), pos);

		gubo.proj = glm::perspective(glm::radians(45.0f),
						swapChainExtent.width / (float) swapChainExtent.height,
						0.1f, 10.0f);
		gubo.proj[1][1] *= -1;

		vkMapMemory(device, DS_global.uniformBuffersMemory[0][currentImage], 0,
			sizeof(gubo), 0, &data);
		memcpy(data, &gubo, sizeof(gubo));
		vkUnmapMemory(device, DS_global.uniformBuffersMemory[0][currentImage]);
		

		// Here is where you actually update your uniforms

		//For the Golden Key
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, deltaKey[1], -8.0f));
		vkMapMemory(device, DS_GKey.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_GKey.uniformBuffersMemory[0][currentImage]);

		//For the Copper Key
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, deltaKey[0], 3.0f));
		vkMapMemory(device, DS_CKey.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_CKey.uniformBuffersMemory[0][currentImage]);

		//For the Floor
		ubo.model = glm::mat4(1.0f);
		vkMapMemory(device, DS_Floor.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Floor.uniformBuffersMemory[0][currentImage]);

		//For the Lever
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(2.50f, 0.507397f, 0.2f)) *
			glm::rotate(glm::mat4(1.0f), -leverAngle[0], glm::vec3(0.0f, 0.0f, 1.0f));
		vkMapMemory(device, DS_Lever1.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Lever1.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(6.10f, 0.507397f, 3.50f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), -leverAngle[1], glm::vec3(0.0f, 0.0f, 1.0f));
		vkMapMemory(device, DS_Lever2.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Lever2.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(1.15f, 0.507397f, -1.50f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), -leverAngle[2], glm::vec3(0.0f, 0.0f, 1.0f));
		vkMapMemory(device, DS_Lever3.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Lever3.uniformBuffersMemory[0][currentImage]);

		//For the Doors
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, -2.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_Door1.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Door1.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 0.0f, 8.0f));
		vkMapMemory(device, DS_Door2.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Door2.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, 0.0f, 3.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_Door3.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Door3.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, 0.0f, 4.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_Door4.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Door4.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 3.0f));
		vkMapMemory(device, DS_Door5.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Door5.uniformBuffersMemory[0][currentImage]);

		//For the Single Door
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, deltaDoor[0], 0.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 3.0f));
		vkMapMemory(device, DS_SDoor1.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SDoor1.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, deltaDoor[1], 3.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_SDoor2.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SDoor2.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, deltaDoor[2], -2.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_SDoor3.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SDoor3.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, deltaDoor[3], 8.0f));
		vkMapMemory(device, DS_SDoor4.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SDoor4.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, deltaDoor[4], 4.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_SDoor5.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SDoor5.uniformBuffersMemory[0][currentImage]);

		//For the Ceiling
		ubo.model = glm::mat4(1.0f);
		vkMapMemory(device, DS_Ceiling.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Ceiling.uniformBuffersMemory[0][currentImage]);

		//For the Walls East
		ubo.model = glm::mat4(1.0f);
		vkMapMemory(device, DS_WallE.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_WallE.uniformBuffersMemory[0][currentImage]);

		//For the Walls Nord
		ubo.model = glm::mat4(1.0f);
		vkMapMemory(device, DS_WallN.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_WallN.uniformBuffersMemory[0][currentImage]);

		//For the Walls Sud
		ubo.model = glm::mat4(1.0f);
		vkMapMemory(device, DS_WallS.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_WallS.uniformBuffersMemory[0][currentImage]);

		//For the Walls West
		ubo.model = glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f);
		vkMapMemory(device, DS_WallW.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_WallW.uniformBuffersMemory[0][currentImage]);

		//For the texts
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(2.4f, deltaInteract[0], 2.3f));
		vkMapMemory(device, DS_Interact1.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Interact1.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(8.30f, deltaInteract[1], 3.70f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_Interact2.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Interact2.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(3.30f, deltaInteract[2], -1.25f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_Interact3.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Interact3.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(6.35f, deltaInteract[3], 8.20f));
		vkMapMemory(device, DS_Interact4.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Interact4.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, deltaInteract[4], 3.25f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_Interact5.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Interact5.uniformBuffersMemory[0][currentImage]);


		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(6.35f, deltaNeedKey[0], 8.20f));
		vkMapMemory(device, DS_NeedKey1.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_NeedKey1.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, deltaNeedKey[1], 3.25f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_NeedKey2.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_NeedKey2.uniformBuffersMemory[0][currentImage]);

	}

	// Setting the MAP matrix from the txt file with all the positions
	void setMap() {

		std::fstream newfile;
		newfile.open(MAP_PATH, std::ios::in);

		if (newfile.is_open()) {

			std::string tp;
			int i = 0;

			// Set a number for each character
			while (getline(newfile, tp)) {
				for (int k = 0; k <= tp.size(); k++) {
					switch (tp.c_str()[k]) {
					case ' ':
						MAP[i][k] = 1;
						break;
					case 'D':
						MAP[i][k] = 2;
						break;
					case 'c':
						MAP[i][k] = 3;
						break;
					case 'g':
						MAP[i][k] = 4;
						break;
					case 'C':
						MAP[i][k] = 5;
						break;
					case 'G':
						MAP[i][k] = 6;
						break;
					case 'L':
						MAP[i][k] = 7;
						break;
					default:
						MAP[i][k] = 0;
					}
				}
				i++;
			}
			newfile.close();
		}
	}

	bool posNum(int n) {
		if (MAP[(int)std::round(-pos.z + 9)][(int)std::round(-pos.x + 6)] == n ||
			MAP[(int)std::round(-pos.z + 8)][(int)std::round(-pos.x + 6)] == n ||
			MAP[(int)std::round(-pos.z + 10)][(int)std::round(-pos.x + 6)] == n ||
			MAP[(int)std::round(-pos.z + 9)][(int)std::round(-pos.x + 7)] == n ||
			MAP[(int)std::round(-pos.z + 9)][(int)std::round(-pos.x + 5)] == n ||
			MAP[(int)std::round(-pos.z + 10)][(int)std::round(-pos.x + 7)] == n ||
			MAP[(int)std::round(-pos.z + 8)][(int)std::round(-pos.x + 5)] == n ||
			MAP[(int)std::round(-pos.z + 10)][(int)std::round(-pos.x + 5)] == n ||
			MAP[(int)std::round(-pos.z + 8)][(int)std::round(-pos.x + 7)] == n)
			return true;
		else
			return false;
	}

	// Check if the new position is a possible one or not
	bool possiblePos(glm::vec3 pos, float margin) {
		bool res = false;
		pos += margin * glm::vec3(glm::rotate(glm::mat4(1.0f), -glm::radians(ang.y),
			glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0, 0, 1, 1));
		pos += margin * glm::vec3(glm::rotate(glm::mat4(1.0f), -glm::radians(ang.y),
			glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1, 0, 0, 1));

		// Check if it's in the map
		if (MAP[(int)std::round(-pos.z + 9)][(int)std::round(-pos.x + 6)] > 0) {

			pos -= margin * glm::vec3(glm::rotate(glm::mat4(1.0f), -glm::radians(ang.y),
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0, 0, 1, 1));
			pos -= margin * glm::vec3(glm::rotate(glm::mat4(1.0f), -glm::radians(ang.y),
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1, 0, 0, 1));

			// Check if it's in front of a closed door
			if (MAP[(int)std::round(-pos.z + 9)][(int)std::round(-pos.x + 6)] == 2 ||
				MAP[(int)std::round(-pos.z + 9)][(int)std::round(-pos.x + 6)] == 3 ||
				MAP[(int)std::round(-pos.z + 9)][(int)std::round(-pos.x + 6)] == 4) {
				res = false;
			}
			else {
				res = true;
			}

			// Removing the text objects
			if (MAP[(int)std::round(-pos.z + 9)][(int)std::round(-pos.x + 6)] != 7 ||
				MAP[(int)std::round(-pos.z + 9)][(int)std::round(-pos.x + 6)] != 3 ||
				MAP[(int)std::round(-pos.z + 9)][(int)std::round(-pos.x + 6)] != 4) {
				for (int k = 0; k < 5; k++) {
					if (k < 3)
						deltaNeedKey[k] = -10.0f;

					deltaInteract[k] = -10.0f;
				}
			}
		}

		// Adding text objects if it's near to a lever
		if (posNum(7)) {
			switch ((int)std::round(-pos.z + 9)) {
			case 11:
				deltaInteract[0] = 0.3f;
				break;
			case 13:
				deltaInteract[1] = 0.3f;
				break;
			case 8:
				deltaInteract[2] = 0.3f;
				break;
			}
		}

		// Adding text objects if it's near to a door closed by a key
		if (posNum(3)) {
			if (hasKey[0] == 0)
				deltaNeedKey[0] = 0.3f;
			else
				deltaInteract[3] = 0.3f;
		}
		if (posNum(4)) {
			if (hasKey[1] == 0)
				deltaNeedKey[1] = 0.3f;
			else
				deltaInteract[4] = 0.3f;
		}

		return res;
	}

	void interact() {

		int i = -1;

		// Switch lever by position
		if (posNum(7)) {
			switch ((int)std::round(-pos.z + 9)) {
			case 11:
				if (MAP[12][10] == 2) {
					MAP[12][10] = 8;
				}
				else if (MAP[12][10] == 8) {
					MAP[12][10] = 2;
				}
				i = 0;
				break;
			case 13:
				if (MAP[12][15] == 2) {
					MAP[12][15] = 8;
				}
				else if (MAP[12][15] == 8) {
					MAP[12][15] = 2;
				}
				i = 1;
				break;
			case 8:
				if (MAP[7][10] == 2) {
					MAP[7][10] = 8;
				}
				else if (MAP[7][10] == 8) {
					MAP[7][10] = 2;
				}
				i = 2;
				break;
			default:
				break;
			}
		}

		// Collect the keys
		if (posNum(5)) {
			hasKey[0] = 1;
			deltaKey[0] = -0.2;
		}
		else if (posNum(6)) {
			hasKey[1] = 1;
			deltaKey[1] = -0.2;
		}

		if (posNum(3) && hasKey[0] == 1) {
			if (MAP[17][13] == 3) {
				MAP[17][13] = 8;
			}
			else if (MAP[17][13] == 8) {
				MAP[17][13] = 3;
			}
			i = 3;
		}
		if (posNum(4) && hasKey[1] == 1) {
			if (MAP[13][18] == 4) {
				MAP[13][18] = 8;
			}
			else if (MAP[13][18] == 8) {
				MAP[13][18] = 4;
			}
			i = 4;
		}

		// Animation of lever and doors
		if (doorStatus[i] == 0) {
			doorStatus[i] = 1;
		}
		else
			doorStatus[i] = 0;
		
		switch (doorStatus[i])
		{
		case 0:
			deltaDoor[i] = 0.0f;
			if(i<=2)
				leverAngle[i] = 0.0f;
			break;
		case 1:
			deltaDoor[i] = 0.8f;
			if(i<=2)
				leverAngle[i] = 30.0f;
			break;
		default:
			break;
		}
	}
};

int main() {

    MyProject app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}