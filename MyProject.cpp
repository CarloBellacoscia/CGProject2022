// This has been adapted from the Vulkan tutorial

#include "MyProject.hpp"

const std::string BODY_MODEL_PATH = "models/SlotBody.obj";
const std::string BODY_TEXTURE_PATH = "textures/SlotBody.png";
const std::string HANDLE_MODEL_PATH = "models/SlotHandle.obj";
const std::string HANDLE_TEXTURE_PATH = "textures/SlotHandle.png";
const std::string WHEEL_MODEL_PATH = "models/SlotWheel.obj";
const std::string WHEEL_TEXTURE_PATH = "textures/SlotWheel.png";


const std::string KEY_MODEL_PATH = "models/key.obj";
const std::string GKEY_TEXTURE_PATH = "textures/GoldKey.png";
const std::string CKEY_TEXTURE_PATH = "textures/CopperKey.png";

const std::string FLOOR_MODEL_PATH = "models/floor.obj";
const std::string FLOOR_TEXTURE_PATH = "textures/EOBSLM04.png";

const std::string LEVER_MODEL_PATH = "models/lever.obj";
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

glm::vec3 ang = glm::vec3(10.0f, 90.0f, 0.0f);
glm::vec3 pos = glm::vec3(1.0f, -0.75f, 0.0f);

glm::vec3 dirX = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 dirZ = glm::vec3(0.0f, 0.0f, 1.0f);


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

	// Models, textures and Descriptors (values assigned to the uniforms)
	/*Model M_SlBody;
	Texture T_SlBody;
	DescriptorSet DS_SlBody;	// instance DSLobj

	Model M_SlHandle;
	Texture T_SlHandle;
	DescriptorSet DS_SlHandle;	// instance DSLobj

	Model M_SlWheel;
	Texture T_SlWheel;
	DescriptorSet DS_SlWheel1;	// instance DSLobj
	DescriptorSet DS_SlWheel2;	// instance DSLobj
	DescriptorSet DS_SlWheel3;	// instance DSLobj*/

	
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
	DescriptorSet DS_Lever;	// instance DSLobj

	Model M_Door;
	Texture T_Door;
	DescriptorSet DS_Door;	// instance DSLobj

	Model M_SDoor;
	Texture T_SDoor;
	DescriptorSet DS_SDoor;	// instance DSLobj

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

	DescriptorSet DS_global;	// instance DSLglobal
	
	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 1500;
		windowHeight = 750;
		windowTitle = "Dungeon";
		initialBackgroundColor = {0.01f, 0.03f, 0.01f, 1.0f};
		
		// Descriptor pool sizes (CHANGE BASED ON THE NUMEBRE OF THE OBJECTS)
		uniformBlocksInPool = 13;
		texturesInPool = 11;
		setsInPool = 14;
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

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, "shaders/vert.spv", "shaders/frag.spv", {&DSLglobal, &DSLobj});

		// Models, textures and Descriptors (values assigned to the uniforms)
		/*M_SlBody.init(this, BODY_MODEL_PATH);
		T_SlBody.init(this, BODY_TEXTURE_PATH);
		DS_SlBody.init(this, &DSLobj, {
		// the second parameter, is a pointer to the Uniform Set Layout of this set
		// the last parameter is an array, with one element per binding of the set.
		// first  elmenet : the binding number
		// second element : UNIFORM or TEXTURE (an enum) depending on the type
		// third  element : only for UNIFORMs, the size of the corresponding C++ object
		// fourth element : only for TEXTUREs, the pointer to the corresponding texture object
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T_SlBody}
				});

		M_SlHandle.init(this, HANDLE_MODEL_PATH);
		T_SlHandle.init(this, HANDLE_TEXTURE_PATH);
		DS_SlHandle.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_SlHandle}
			});

		M_SlWheel.init(this, WHEEL_MODEL_PATH);
		T_SlWheel.init(this, WHEEL_TEXTURE_PATH);
		DS_SlWheel1.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_SlWheel}
			});
		DS_SlWheel2.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_SlWheel}
			});
		DS_SlWheel3.init(this, &DSLobj, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_SlWheel}
			});*/

		
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
		DS_Lever.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Lever}
			});

		M_Door.init(this, DOOR_MODEL_PATH);
		T_Door.init(this, DOOR_TEXTURE_PATH);
		DS_Door.init(this, &DSLobj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &T_Door}
			});

		M_SDoor.init(this, SINGLE_DOOR_MODEL_PATH);
		T_SDoor.init(this, SINGLE_DOOR_TEXTURE_PATH);
		DS_SDoor.init(this, &DSLobj, {
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
		


		DS_global.init(this, &DSLglobal, {
				{0, UNIFORM, sizeof(globalUniformBufferObject), nullptr}
			});
	}

	// Here you destroy all the objects you created!		
	void localCleanup() {
		/*DS_SlBody.cleanup();
		T_SlBody.cleanup();
		M_SlBody.cleanup();

		DS_SlHandle.cleanup();
		T_SlHandle.cleanup();
		M_SlHandle.cleanup();

		DS_SlWheel1.cleanup();
		DS_SlWheel2.cleanup();
		DS_SlWheel3.cleanup();
		T_SlWheel.cleanup();
		M_SlWheel.cleanup();*/

		
		DS_GKey.cleanup();
		T_GKey.cleanup();
		DS_CKey.cleanup();
		T_CKey.cleanup();
		M_Key.cleanup();

		DS_Floor.cleanup();
		T_Floor.cleanup();
		M_Floor.cleanup();

		DS_Lever.cleanup();
		T_Lever.cleanup();
		M_Lever.cleanup();

		DS_Door.cleanup();
		T_Door.cleanup();
		M_Door.cleanup();

		DS_SDoor.cleanup();
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
				
		/*VkBuffer vertexBuffers[] = {M_SlBody.vertexBuffer};
		// property .vertexBuffer of models, contains the VkBuffer handle to its vertex buffer
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		// property .indexBuffer of models, contains the VkBuffer handle to its index buffer
		vkCmdBindIndexBuffer(commandBuffer, M_SlBody.indexBuffer, 0,
								VK_INDEX_TYPE_UINT32);

		// property .pipelineLayout of a pipeline contains its layout.
		// property .descriptorSets of a descriptor set contains its elements.
		vkCmdBindDescriptorSets(commandBuffer,
						VK_PIPELINE_BIND_POINT_GRAPHICS,
						P1.pipelineLayout, 1, 1, &DS_SlBody.descriptorSets[currentImage],
						0, nullptr);
						
		// property .indices.size() of models, contains the number of triangles * 3 of the mesh.
		vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M_SlBody.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffers2[] = { M_SlHandle.vertexBuffer };
		VkDeviceSize offsets2[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
		vkCmdBindIndexBuffer(commandBuffer, M_SlHandle.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_SlHandle.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_SlHandle.indices.size()), 1, 0, 0, 0);

		VkBuffer vertexBuffers3[] = { M_SlWheel.vertexBuffer };
		VkDeviceSize offsets3[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers3, offsets3);
		vkCmdBindIndexBuffer(commandBuffer, M_SlWheel.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_SlWheel1.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_SlWheel.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_SlWheel2.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_SlWheel.indices.size()), 1, 0, 0, 0);

		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DS_SlWheel3.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M_SlWheel.indices.size()), 1, 0, 0, 0);*/

		
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
			P1.pipelineLayout, 1, 1, &DS_Lever.descriptorSets[currentImage],
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
			P1.pipelineLayout, 1, 1, &DS_Door.descriptorSets[currentImage],
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
			P1.pipelineLayout, 1, 1, &DS_SDoor.descriptorSets[currentImage],
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
					
		static int state = 0;	// 0 -> everything is still
								// 1 -> one wheel is turning
								// 2 -> two wheels are turning
								// 3 -> three wheels are turning

		static float debounce = time;
		static float ang1 = 0.0f;
		static float ang2 = 0.0f;
		static float ang3 = 0.0f;
		static float angHandle = 0.0f;

		/*if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			if (time - debounce > 0.33) {
				debounce = time;

				if (state == 0) {
					state = 3;
				}
				else {
					state--;
				}
			}
		}*/

		const float speed = 0.7;
		const float angSpeed = 25.0;

		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			ang.y -= deltaT * angSpeed;
			//dirX = glm::vec3(glm::rotate(glm::mat4(1.0f), -deltaT * angSpeed, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(dirX, 1.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			ang.y += deltaT * angSpeed;
			//dirX = glm::vec3(glm::rotate(glm::mat4(1.0f), -deltaT * angSpeed, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(dirX, 1.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_UP)) {
			ang.x -= deltaT * angSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN)) {
			ang.x += deltaT * angSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_R)) {
			ang = glm::vec3(10.0f, 90.0f, 0.0f);
			pos = glm::vec3(1.0f, -0.75f, 0.0f);
		}

		if (glfwGetKey(window, GLFW_KEY_Q)) {
			ang = glm::vec3(10.0f, 90.0f, 0.0f);
		}

		if (glfwGetKey(window, GLFW_KEY_W)) {
			//pos -= deltaT * speed * dirX;
			pos -= speed * glm::vec3(glm::rotate(glm::mat4(1.0f), ang.x,
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0, 0, 1, 1)) * deltaT;
		}

		if (glfwGetKey(window, GLFW_KEY_S)) {
			pos += speed * glm::vec3(glm::rotate(glm::mat4(1.0f), ang.x,
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0, 0, 1, 1)) * deltaT;
		}

		if (glfwGetKey(window, GLFW_KEY_D)) {
			pos += speed * glm::vec3(glm::rotate(glm::mat4(1.0f), ang.x,
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1, 0, 0, 1)) * deltaT;

		}

		if (glfwGetKey(window, GLFW_KEY_A)) {
			pos -= speed * glm::vec3(glm::rotate(glm::mat4(1.0f), ang.x,
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1, 0, 0, 1)) * deltaT;
		}

		if (state == 3) {
			angHandle = 1.0f;
			ang3 += deltaT;
		}
		if (state >= 2) {
			ang2 += deltaT;
		}
		if (state >= 1) {
			ang1 += deltaT;
		}
		if(state == 0) {
			angHandle = 0.0f;
		}
					
		globalUniformBufferObject gubo{}; 
		UniformBufferObject ubo{};
		/* CONSTANT ROTATION OF THE OBJECT
		ubo.model = glm::rotate(glm::mat4(1.0f),
								time * glm::radians(90.0f),
								glm::vec3(0.0f, 0.0f, 1.0f));
		*/
		void* data;
		/*gubo.view = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f),
							   glm::vec3(3.0f, 0.0f, 0.0f),
							   glm::vec3(0.0f, 1.0f, 0.0f));*/

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

		//For the Slot Body
		/*ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
		vkMapMemory(device, DS_SlBody.uniformBuffersMemory[0][currentImage], 0,
							sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SlBody.uniformBuffersMemory[0][currentImage]);

		//For the Slot Handle
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.0f, -0.15f)) *
					glm::rotate(glm::mat4(1.0f),
						angHandle * glm::radians(90.0f),
						glm::vec3(1.0f, 0.0f, 0.0f));
		vkMapMemory(device, DS_SlHandle.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SlHandle.uniformBuffersMemory[0][currentImage]);

		//For the Slot Wheel1
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.15f, 0.43f, -0.15f)) *
					glm::rotate(glm::mat4(1.0f),
						ang1 * glm::radians(90.0f),
						glm::vec3(1.0f, 0.0f, 0.0f));
		vkMapMemory(device, DS_SlWheel1.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SlWheel1.uniformBuffersMemory[0][currentImage]);

		//For the Slot Wheel2
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.43f, -0.15f)) *
					glm::rotate(glm::mat4(1.0f),
						ang2 * glm::radians(90.0f),
						glm::vec3(1.0f, 0.0f, 0.0f));
		vkMapMemory(device, DS_SlWheel2.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SlWheel2.uniformBuffersMemory[0][currentImage]);

		//For the Slot Wheel3
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.15f, 0.43f, -0.15f)) *
					glm::rotate(glm::mat4(1.0f),
						ang3 * glm::radians(90.0f),
						glm::vec3(1.0f, 0.0f, 0.0f));
		vkMapMemory(device, DS_SlWheel3.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SlWheel3.uniformBuffersMemory[0][currentImage]);*/

		
		//For the Golden Key
		ubo.model = glm::mat4(1.0f);
		vkMapMemory(device, DS_GKey.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_GKey.uniformBuffersMemory[0][currentImage]);

		//For the Copper Key
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
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
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		vkMapMemory(device, DS_Lever.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Lever.uniformBuffersMemory[0][currentImage]);

		//For the Door
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
		vkMapMemory(device, DS_Door.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Door.uniformBuffersMemory[0][currentImage]);

		//For the Single Door
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
		vkMapMemory(device, DS_SDoor.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SDoor.uniformBuffersMemory[0][currentImage]);

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
		

	}	
};

// This is the main: probably you do not need to touch this!
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