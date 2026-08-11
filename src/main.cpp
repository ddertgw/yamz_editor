#include <yamz_engine/yamz.hpp>
#include <yamz_editor/imgui_helper.hpp>
#include <iostream>
#include <stdexcept>
#include <system_error>

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

constexpr GLfloat points[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f,  -0.5f, 0.0f 
};

constexpr GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

constexpr GLfloat positions_colors[] = {
    //positions             //colors
     0.0f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f
};

constexpr GLfloat positions_colors2[] = {
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
};

constexpr GLuint indices[] = {
    0, 1, 2, 3, 2, 1
};
const char* vertex_shader_src =
    R"( 
        #version 460
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec3 vertex_color;
        out vec3 color;
        uniform mat4 transformation_matrix;
        void main() {
            color = vertex_color;
            gl_Position = transformation_matrix * vec4(vertex_position, 1.0);
        }
    )";
const char* fragment_shader_src =
    R"(
        #version 460
        in vec3 color;
        out vec4 frag_color;
        void main() {
            frag_color = vec4(color, 1.0);
        }
    )";

class YamzEditor : public yamz_core::Application_GL {

    void user_init() final {
        using namespace yamz_core;
        imguitor_.init(up_window_->get_raw_window());
        p_shaders_ = std::make_unique<ShaderProgram>(vertex_shader_src, fragment_shader_src);
        
        BufferLayout bf {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };

        p_pos_colors_vbo_ = std::make_unique<VertexBuffer>(
              positions_colors2
            , sizeof(positions_colors2)
            , bf
            , Usage::Static
        );
        p_index_buffer_ = std::make_unique<ElementBuffer>(
              indices
            , sizeof(indices) / sizeof(GLuint)
            , Usage::Static
        );
        p_vao_ = std::make_unique<VertexArray>();
        p_vao_->add_buffer(*p_pos_colors_vbo_);
        p_vao_->set_element_buffer(*p_index_buffer_);

    }

    void on_update() final {
        using namespace yamz_core;

        glClearColor(0.33f, 0.33f, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        p_shaders_->bind();

        glm::mat4 scale_matrix(
            scale_[0],   0,          0,          0,
            0,          scale_[1],   0,          0,
            0,          0,          scale_[2],   0,
            0,          0,          0,          1
        );

        float rotate_in_radians = glm::radians(rotate_);
        glm::mat4 rotate_matrix(
            cos(rotate_in_radians),     sin(rotate_in_radians),         0,          0,
            -sin(rotate_in_radians),    cos(rotate_in_radians),         0,          0,
            0,                          0,                              1,          0,
            0,                          0,                              0,          1
        );
        glm::mat4 translation_matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translation_[0], translation_[1],  translation_[2], 1
        );

        camera_.set_position_rotation(
            glm::vec3(camera_pos_[0], camera_pos_[1], camera_pos_[2])
            , glm::vec3(camera_rot_[0], camera_rot_[1], camera_rot_[2])
        );
        camera_.set_projection_mode(
            (perspective_camera_
                ? Camera::ProjectionMode::Perspective 
                : Camera::ProjectionMode::Orthogrphic
            ) 
        );

        glm::mat4 transformation_matrix = camera_.get_projection_matrix() * camera_.get_view_matrix() * translation_matrix * rotate_matrix * scale_matrix;
        p_shaders_->set_mat4("transformation_matrix", transformation_matrix);


        p_vao_->bind();
        glDrawElements(GL_TRIANGLES, p_vao_->get_num_elements(), GL_UNSIGNED_INT, nullptr);
        //glDrawArrays(GL_TRIANGLES, 0, 12);

        imguitor_.on_update(
              scale_
            , rotate_
            , translation_
            , camera_pos_
            , camera_rot_
            , perspective_camera_
        );
    }
    
    yamz_editor::ImGuitor imguitor_;

    std::unique_ptr<yamz_core::ShaderProgram> p_shaders_;
    //std::unique_ptr<VertexBuffer> p_points_vbo;
    //std::unique_ptr<VertexBuffer> p_colors_vbo;
    std::unique_ptr<yamz_core::VertexBuffer> p_pos_colors_vbo_;
    std::unique_ptr<yamz_core::ElementBuffer> p_index_buffer_;
    std::unique_ptr<yamz_core::VertexArray> p_vao_;

    float scale_[3] = {1.f, 1.f, 1.f};
    float rotate_ = 0.f;
    float translation_[3] = {0.f, 0.f, 0.f};

    float camera_pos_[3] = {0.f, 0.f, 1.f};
    float camera_rot_[3] = {0.f, 0.f, 0.f};
    bool perspective_camera_ = false;

    yamz_core::Camera camera_ = {yamz_core::Camera::ProjectionMode::Orthogrphic};
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