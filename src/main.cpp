#include <yamz_engine/yamz.hpp>
//#include <yamz_editor/imgui_helper.hpp>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <yamz_engine/detail/cams/camera2.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr unsigned int SCR_WIDTH = 1920;
constexpr unsigned int SCR_HEIGHT = 1080;

constexpr GLfloat vertices[] = {
    // Позиции          // Цвета             // Текстурные координаты
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
};

constexpr GLuint indices[] = {
    0, 1, 3, 1, 2, 3
};

float cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

const char* vertex_shader_src =
    R"( 
        #version 460
        layout(location = 0) in vec3 vertex_position;
        layout (location = 1) in vec2 texCoord;

        out vec2 TexCoord;

        uniform mat4 model;
        uniform mat4 transformation_matrix;

        void main() {
            gl_Position = transformation_matrix * model * vec4(vertex_position, 1.0);
            TexCoord = texCoord;
        }
    )";
const char* fragment_shader_src =
    R"(
        #version 460

        in vec2 TexCoord;

        uniform sampler2D ourTexture;

        out vec4 frag_color;

        void main() {
            frag_color = texture(ourTexture, TexCoord);
        }
    )";

class YamzEditor : public yamz_core::Application_GL {

    void user_init() final {
        using namespace yamz_core;

        glfwSetInputMode(up_window_->get_raw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        dispatcher_.set_listener<EventKeyInput>(
            [this] (EventKeyInput& e) {
                if (e.key >= 0 && e.key < 1024) {
                    if (e.action == GLFW_PRESS)
                        keys_[e.key] = true;
                    else if (e.action == GLFW_RELEASE)
                        keys_[e.key] = false;
                }
            }
        );

        dispatcher_.set_listener<EventMouseInput>(
            [this] (EventMouseInput& e) {
                static bool firstMouse = true;
                if(firstMouse) {
                    lastX_ = e.x_pos;
                    lastY_ = e.y_pos;
                    firstMouse = false;
                }

                float xoffset = static_cast<float>(e.x_pos - lastX_);
                float yoffset = lastY_ - e.y_pos; 
                lastX_ = e.x_pos;
                lastY_ = e.y_pos;
            
                cam.mouse_move(xoffset, yoffset);
            }
        );

        dispatcher_.set_listener<EventScrollInput>(
            [this] (EventScrollInput& e) {
                float new_fov = cam.get_fov() -  static_cast<GLfloat>(e.y_offset);
                if(new_fov <= 1.0f)
                    new_fov = 1.0f;
                if(new_fov >= 80.0f)
                    new_fov = 80.0f;
                cam.set_fov(new_fov);
            }
        );

        dispatcher_.set_listener<EventResizeWindow>(
            [this] (EventResizeWindow& e) {
                cam.set_width(e.width);
                cam.set_height(e.height);
            }
        );

        glEnable(GL_DEPTH_TEST);

        //imguitor_.init(up_window_->get_raw_window());
        p_shaders_ = std::make_unique<ShaderProgram>(vertex_shader_src, fragment_shader_src);
        
        BufferLayout bf {
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };

        p_pos_colors_vbo_ = std::make_unique<VertexBuffer>(
              cube
            , sizeof(cube)
            , bf
            , Usage::Static
        );

        p_vao_ = std::make_unique<VertexArray>();
        p_vao_->add_buffer(*p_pos_colors_vbo_);

        p_texture_ = std::make_unique<Texture2D>("texture.png", true);
        p_texture_->bind(0);
    }

    void on_update() final {
        using namespace yamz_core;

        glClearColor(0.13f, 0.13f, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        p_shaders_->bind();

        glm::mat4 transformation_matrix = cam.get_transform_matrix();
        p_shaders_->set_mat4("transformation_matrix", transformation_matrix);

        p_vao_->bind();
        for(GLuint i = 0; i < 10; i++)
        {
            glm::mat4 model(1);
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i; 
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            p_shaders_->set_mat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        do_movement();

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void do_movement() noexcept {
        if (keys_[GLFW_KEY_W])
            cam.move_forward();
        if (keys_[GLFW_KEY_S])
            cam.move_back();
        if (keys_[GLFW_KEY_A])
            cam.move_left();
        if (keys_[GLFW_KEY_D])
            cam.move_right();
    }

    yamz_core::Camera2 cam = {SCR_WIDTH, SCR_HEIGHT};

    double lastX_ = static_cast<float>(SCR_WIDTH) / 2.0f;
    double lastY_ = static_cast<float>(SCR_HEIGHT) / 2.0f;

    bool keys_[1024];

    //yamz_editor::ImGuitor imguitor_;
    std::unique_ptr<yamz_core::ShaderProgram> p_shaders_;
    //std::unique_ptr<VertexBuffer> p_points_vbo;
    //std::unique_ptr<VertexBuffer> p_colors_vbo;
    std::unique_ptr<yamz_core::VertexBuffer> p_pos_colors_vbo_;
    std::unique_ptr<yamz_core::ElementBuffer> p_index_buffer_;
    std::unique_ptr<yamz_core::VertexArray> p_vao_;
    std::unique_ptr<yamz_core::Texture2D> p_texture_;
};

int main() {
    try {
        auto my_app = std::make_unique<YamzEditor>();
        int result = my_app->start(SCR_WIDTH, SCR_HEIGHT, "yamz_window");
        std::cout << "Result: " << result << std::endl;
    } catch (const std::system_error& e) {
        std::cout << "Catch system_error exception!\n"
                  << "[" << e.code() << "] " << e.what() << std::endl;
        return -2;
    } catch (const std::exception& e) {
        std::cout << "Catch std::exception!\n" << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cout << "Catch unknown exception!" << std::endl;
        return -3;
    }
    return 0;
}