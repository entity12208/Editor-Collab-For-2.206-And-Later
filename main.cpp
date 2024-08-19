#include <Geode/Geode.hpp>
#include <Geode/Modify.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/websocket.hpp>
#include <nlohmann/json.hpp>

USE_GEODE_NAMESPACE();
using json = nlohmann::json;

class $modify(EditorLayer) {
    WebSocketClient* m_collabClient;

    bool init(GJGameLevel* level) {
        if (!EditorLayer::init(level)) return false;

        // Connect to the collaboration server
        m_collabClient = WebSocketClient::create("wss://your-collab-server.com");
        m_collabClient->onMessage([this](std::string const& message) {
            this->handleCollabMessage(message);
        });
        
        return true;
    }

    void update(float dt) override {
        // Regular update checks
        EditorLayer::update(dt);
    }

    void draw() override {
        // Render updates if needed
        EditorLayer::draw();
    }

    void handleCollabMessage(const std::string& message) {
        try {
            auto data = json::parse(message);
            if (data["type"] == "object_update") {
                auto objData = data["object"];
                int objectId = objData["id"];
                // Update object in the editor with new data
                GameObject* obj = this->getObjectById(objectId);
                if (obj) {
                    obj->setPosition({ objData["x"], objData["y"] });
                    obj->setRotation(objData["rotation"]);
                    obj->setScale(objData["scale"]);
                    // Handle other properties...
                }
            } else if (data["type"] == "object_delete") {
                int objectId = data["id"];
                // Delete object from the editor
                this->removeObjectById(objectId);
            } else if (data["type"] == "new_object") {
                auto objData = data["object"];
                // Create a new object in the editor
                GameObject* obj = GameObject::create(objData["type"]);
                obj->setPosition({ objData["x"], objData["y"] });
                obj->setRotation(objData["rotation"]);
                obj->setScale(objData["scale"]);
                // Add the object to the editor
                this->addObject(obj);
            }
        } catch (const std::exception& e) {
            Mod::get()->log() << "Error parsing collaboration message: " << e.what();
        }
    }

    bool onClose() override {
        // Send a disconnect message to the server
        if (m_collabClient) {
            m_collabClient->send(R"({"type": "disconnect"})");
            m_collabClient->close();
        }
        
        return EditorLayer::onClose();
    }

private:
    GameObject* getObjectById(int id) {
        for (auto obj : m_objects) {
            if (obj->getObjectID() == id) {
                return obj;
            }
        }
        return nullptr;
    }

    void removeObjectById(int id) {
        auto obj = getObjectById(id);
        if (obj) {
            this->removeObject(obj);
        }
    }
};

class EditorCollabMod : public Mod {
    void onLoad() override {
        Mod::get()->log() << "EditorCollab: Mod loaded!";
    }

    void onUnload() override {
        Mod::get()->log() << "EditorCollab: Mod unloaded!";
    }
};

GEODE_MODIFY_CLASS(EditorLayer)
GEODE_MOD(EditorCollabMod)
