#include "CScene.hpp"
#include <fstream>
#include <utilslib/logger.hpp>

void CCameraManager::addCameraPoint(glm::vec3 position, glm::vec3 m_rotation)
{
    auto point = CCameraPoint(position, m_rotation);
    this->m_points.push_back(point);
}

void CCameraManager::addNewTrack()
{
    this->m_tracks.push_back(CCameraTrack());
}


void CScene::save(const std::string path)
{
    auto jsonObject = m_cameraManager.serialize();
    std::string str = jsonObject.dump();

    std::ofstream outputFile(path);
    outputFile << str;

    utilslib::Logger::getInfo() << "CScene::save() " << path << ": " << str << std::endl;
}
void CScene::load(const std::string path)
{
    try {
        std::ifstream inputFile(path);
        if(inputFile.is_open())
        {
            std::string str;
            inputFile >> str;
            nlohmann::json jsonObj = nlohmann::json::parse(str);
            utilslib::Logger::getInfo() << "Parsed " << path << " successfully " << std::endl;
            m_cameraManager.deserialize(jsonObj);
        } else {
            utilslib::Logger::getInfo() << "CScene::load() failed to load file " << path << std::endl;
        }
    } 
    catch(nlohmann::json::exception& e)
    {
        utilslib::Logger::getInfo() << "Exception: " << e.what() << std::endl;
    }
    catch(...)
    {

    }
}