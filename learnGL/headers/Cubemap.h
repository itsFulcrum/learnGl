#ifndef CUBEMAP_H
#define CUBEMAP_H

#pragma once
#include <glad/glad.h>
#include <iostream>
#include <vector>


// image loader libary
#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
	#include<stb_image.h>
#endif // !STB_IMAGE_IMLEMENTATION

#include <Shader.h>
#include <Object.h>

class Cubemap
{
public:
	unsigned int ID;
	unsigned int panoramaImageID;

private:
	bool m_hasAlphaChannel = false;
	int m_channelsAmount = 0;

public:

	Cubemap(std::string path, unsigned int textureResolution)
	{

		// generate normal opengl texture from the hdri panorama
		glGenTextures(1, &panoramaImageID);
		glBindTexture(GL_TEXTURE_2D, panoramaImageID);

		stbi_set_flip_vertically_on_load(false);

		int width, height, channelsAmount;
		// replace paths list with normal string not list of strings
		unsigned char* panoramaImageData = stbi_load(path.c_str(), &width, &height, &channelsAmount, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, panoramaImageData);

		if (!panoramaImageData)
		{
			std::cout << "ERROR::CUBEMAP:: COULD_NOT_LOAD_IMAGE at path: " << path << std::endl;
			stbi_image_free(panoramaImageData);
			return;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(panoramaImageData);
		

		panoramaToCubemap(textureResolution);
	};

	void panoramaToCubemap(unsigned int textureRes)
	{
		// to create a cubemap from an equirectangular panorama Hdri image I Create a framebuffer that renders to a texture.
		// the using a clever shader by the khronos group we loop through all 6 sides of the cube/cubemap textures and render each directly to the cubemap object;
		// For this we need said shaders and a dummy object to make a drawcall but as i understad it it doesn't matter what the object is since it's all handled by the shader
		// we just need to make some drawcall
		
	
		glViewport(0, 0, textureRes, textureRes);


		// fill in filepath
		Shader panoramaToCubemapShader("shaders/panoramaToCubemap.vert","shaders/panoramaToCubemap.frag");
		
		Object dummyObj("fbx/skyboxMesh.fbx");
		
		


		unsigned int fbo;
		glGenFramebuffers(1, &fbo);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

		// create cubemap
		for (int i = 0; i < 6; ++i) 
		{
			auto data = std::vector<unsigned char>();
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, textureRes, textureRes, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


		

		for(int i = 0;i < 6;i++)
		{
			int side = i;

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, ID, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				std::cout << "ERROR::CUBEMAP::FRAMEBUFFER_NOT_COMPLETE" << std::endl;
				
				glDeleteFramebuffers(1, &fbo);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				
				return;
			}

			glBindTexture(GL_TEXTURE_CUBE_MAP, ID);


			glViewport(0, 0, textureRes, textureRes);
			
			glClearColor(1.0f, 0.0f, 1.0f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			

			panoramaToCubemapShader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, panoramaImageID);

			panoramaToCubemapShader.setUniformInt("_panorama", 0);
			panoramaToCubemapShader.setUniformInt("_currentFace", side);


			dummyObj.Render(panoramaToCubemapShader, glm::mat4(0.0f), glm::mat4(0.0f));

		}


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);


		// i should delete dummy obj and panorama shader here as otherwise there will be no way to access them after this has run

		//delete& dummyObj;
		//delete& panoramaToCubemapShader;
		
		// return to default framebuffer
		glDeleteFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Bind()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	}

	
	int GetChannelsAmount()
	{
		return m_channelsAmount;
	}

private:
	


};


#endif 
