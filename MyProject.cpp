// This has been adapted from the Vulkan tutorial

#include "MyProject.hpp"


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
glm::vec3 pos = glm::vec3(0.85797f, -0.75f, -2.81876f);



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
		setsInPool = 24;
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

		const float speed = 0.7;
		const float angSpeed = 25.0;

		if (glfwGetKey(window, GLFW_KEY_R)) {
			ang = glm::vec3(10.0f, 90.0f, 0.0f);
			pos = glm::vec3(0.85797f, -0.75f, -2.81876f);
		}

		if (glfwGetKey(window, GLFW_KEY_Q)) {
			ang = glm::vec3(10.0f, 90.0f, 0.0f);
		}

		if (glfwGetKey(window, GLFW_KEY_W)) {
			pos.x -= deltaT * speed;
		}

		if (glfwGetKey(window, GLFW_KEY_S)) {
			pos.x += deltaT * speed;
		}

		if (glfwGetKey(window, GLFW_KEY_D)) {
			pos.z -= deltaT * speed;

		}

		if (glfwGetKey(window, GLFW_KEY_A)) {
			pos.z += deltaT * speed;
		}

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

		//For the Golden Key
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, -8.0f));
		vkMapMemory(device, DS_GKey.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_GKey.uniformBuffersMemory[0][currentImage]);

		//For the Copper Key
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, 3.0f));
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
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(2.40f, 0.507397f, 2.2f));
		vkMapMemory(device, DS_Lever1.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Lever1.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(8.30f, 0.507397f, 3.60f)) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_Lever2.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Lever2.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(3.30f, 0.507397f, -1.40f)) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, -2.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_SDoor1.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SDoor1.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 0.0f, 8.0f));
		vkMapMemory(device, DS_SDoor2.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SDoor2.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, 0.0f, 3.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_SDoor3.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SDoor3.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, 0.0f, 4.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		vkMapMemory(device, DS_SDoor4.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_SDoor4.uniformBuffersMemory[0][currentImage]);

		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 3.0f));
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