//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/architecture.h"
#include <rttr/registration>

#include <utility>


std::shared_ptr<Component> Architecture::get_component(const std::string& name) {
    return components_[name];
}

void Architecture::add_component(const std::string& name, std::shared_ptr<Component> component) {
    components_[name] = std::move(component);
}

void Architecture::add_components(std::unordered_map<std::string, std::shared_ptr<Component>> components) {
    components_.merge(components);
}

rttr::variant Architecture::components_converter(json& j) {
    std::unordered_map<std::string, std::shared_ptr<Component>> components;
    std::string component;
    try {
        for (auto& [_, value] : j.items()) {
            if (!value.contains("_name")) {
                std::cerr << "Component does not have a \"_name\" field set" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::string name = value["_name"];
            if (!value.contains("_component")) {
                std::cerr << "Component does not have a \"_component\" field set" << std::endl;
                exit(EXIT_FAILURE);
            }
            component = value["_component"];
            std::shared_ptr<Component> instance = Parser::get_instance().create_instance(component, value).get_value<std::shared_ptr<Component>>();
            components[name] = instance;
        }
        return rttr::variant(components);
    } catch (json::type_error&) {
        std::cerr << "[Error] Could not parse JSON for component \"" << component << "\"" << std::endl;
    } catch (json::parse_error&) {
        std::cerr << "[Error] Could not parse JSON for component \"" << component << "\"" << std::endl;
    }

    return rttr::variant();
}

void Architecture::update_cycles(int cycles) {
#ifdef TRACK_STATISTICS
    counter_ += cycles;
#endif
}

#ifdef TRACK_STATISTICS

void Architecture::update_static() {
    double time = (1.0 / config_->get_clock_rate()) * counter_.get_cycles();
    for (auto& [_, component] : components_) {
        component->update_static(time);
    }
}

void Architecture::print_stats(int rows, int cols) {

    std::cout << "=============================="
              << "\n";
    std::cout << "Results"
              << "\n";
    std::cout << "=============================="
              << "\n";
    std::cout << "Clock rate: " << config_->get_clock_rate() << " Hz\n";
    std::cout << "Process node: " << config_->get_process_node() << " nm\n";
    std::cout << "Voltage: " << config_->get_voltage() << " V\n";
    std::cout << "Temperature: " << config_->get_temperature() << " C\n";
    std::cout << "=============================="
              << "\n";

    std::cout << "Cycle count: " << counter_.get_cycles() << "\n";
    double exec_time = ((double)counter_.get_cycles() / config_->get_clock_rate());
    std::cout << "Device execution time: " << exec_time << " s\n";

    long transistor_count = 0;
    for (auto& [_, component] : components_) {
        transistor_count += cv::sum(component->get_transistor_count_array())[0];
    }
    std::cout << "Architecture transistor count: " << transistor_count << "\n";
//    std::cout << "Average transistor count per PE: " << transistor_count / (num_pes) << " \n";

    double static_energy = 0.0;
    for (auto& [_, component] : components_) {
        static_energy += cv::sum(component->get_static_energy_array())[0];
    }
    double static_power = static_energy/exec_time;

    double dynamic_energy = 0.0;
    for (auto& [_, component] : components_) {
        dynamic_energy += cv::sum(component->get_dynamic_energy_array())[0];
    }
    double dynamic_power = dynamic_energy/exec_time;

    std::cout << "Architecture static energy: " << static_energy << " J\n";
    std::cout << "Architecture static power: " << static_power << " W\n";
    std::cout << "Architecture dynamic energy: " << dynamic_energy << " J\n";
    std::cout << "Architecture dynamic power: " << dynamic_power << " W\n";
    std::cout << "Architecture total power: " << static_power + dynamic_power << " W\n";
    double width = 0;
    for (auto& [_, component] : components_) {
        width += component->get_width();
    }
    double height = 0;
    for (auto& [_, component] : components_) {
        height += component->get_height();
    }
    std::cout << "Single PE dimensions: " << width << "μm x " << height << "μm\n";
    std::cout << "Area of a single PE: " << width*0.001 * height * 0.001 << " mm^2" << "\n";
    // TODO should be based off row_stride, col_stride not rows, cols
    std::cout << "Chip total dimensions: " << width*(rows*1.1) << "μm x " << height*(cols * 1.1) << "μm\n";
    std::cout << "Chip area: " << width*(rows*1.15) * 0.001 * height*(cols * 1.15) * 0.001 << " mm^2" << std::endl;

}

unsigned long long Architecture::get_cycles() {
    return counter_.get_cycles();
}

//
void Architecture::write_stats(int rows, int cols, json& j) {
    j["Clock rate"] = config_->get_clock_rate();
    j["Process node"] = config_->get_process_node();
    j["Voltage"] =  config_->get_voltage();
    j["Temperature"] = config_->get_temperature();
    j["Cycle count"] = counter_.get_cycles();
    double exec_time = ((double)counter_.get_cycles() / config_->get_clock_rate());
    j["Device execution time"] = exec_time;

    long transistor_count = 0;
    for (auto& [_, component] : components_) {
        transistor_count += cv::sum(component->get_transistor_count_array())[0];
    }
    j["Architecture transistor count"] = transistor_count;
//    j["Average transistor count per PE"] = transistor_count / (num_pes);

    double static_energy = 0.0;
    for (auto& [_, component] : components_) {
        static_energy += cv::sum(component->get_static_energy_array())[0];
    }
    double static_power = static_energy/exec_time;

    double dynamic_energy = 0.0;
    for (auto& [_, component] : components_) {
        dynamic_energy += cv::sum(component->get_dynamic_energy_array())[0];
    }
    double dynamic_power = dynamic_energy/exec_time;

    j["Architecture static energy"] = static_energy;
    j["Architecture static power"] = static_power;
    j["Architecture dynamic energy"] = dynamic_energy;
    j["Architecture dynamic power"] = dynamic_power;
    j["Architecture total power"] = static_power + dynamic_power;
    double width = 0;
    for (auto& [_, component] : components_) {
        width += component->get_width();
    }
    double height = 0;
    for (auto& [_, component] : components_) {
        height += component->get_height();
    }
    j["Single PE width"] = width;
    j["Single PE height"] = height;
    j["Area of a single PE"] = width*0.001 * height * 0.001;
    // TODO should be based off row_stride, col_stride not rows, cols
    j["Chip total width"] = width*(rows*1.1);
    j["Chip total height"] = height*(cols*1.1);
    j["Chip area"] = width*(rows*1.15) * 0.001 * height*(cols * 1.15) * 0.001;
}
#endif

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<Architecture>("Architecture")
        .method("components_converter", &Architecture::components_converter);

};
