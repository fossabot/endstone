//
// Created by Vincent on 20/08/2023.
//

#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"

PluginManager::PluginManager(Server &server) : server_(server)
{
}

void PluginManager::registerLoader(const std::shared_ptr<PluginLoader> &loader)
{
    auto patterns = loader->getPluginFilters();
    for (const auto &pattern : patterns)
    {
        fileAssociations_[pattern] = loader;
    }
}

Plugin *PluginManager::getPlugin(const std::string &name) const
{
    auto it = lookupNames_.find(name);
    if (it != lookupNames_.end())
    {
        return it->second;
    }
    return nullptr;
}

std::vector<Plugin *> PluginManager::getPlugins() const
{
    std::vector<Plugin *> plugins(plugins_.size());
    for (const auto &plugin : plugins_)
    {
        plugins.push_back(plugin.get());
    }
    return plugins;
}

bool PluginManager::isPluginEnabled(const std::string &name) const
{
    return isPluginEnabled(getPlugin(name));
}

bool PluginManager::isPluginEnabled(Plugin *plugin) const
{
    if (!plugin)
    {
        return false;
    }

    // Check if the plugin exists in the vector
    auto it = std::find_if(
        plugins_.begin(), plugins_.end(), [plugin](const std::unique_ptr<Plugin> &p) { return p.get() == plugin; });

    // If plugin is in the vector and is enabled, return true
    return it != plugins_.end() && plugin->isEnabled();
}

Plugin *PluginManager::loadPlugin(const std::filesystem::path &file)
{
    if (!exists(file))
    {
        throw std::runtime_error("Provided file does not exist.");
    }

    for (const auto &[pattern, loader] : fileAssociations_)
    {
        std::regex r(pattern);
        if (std::regex_match(file.string(), r))
        {
            Plugin *plugin = loader->loadPlugin(file.string());

            if (plugin)
            {
                plugins_.push_back(std::unique_ptr<Plugin>(plugin));
                lookupNames_[plugin->getDescription().getName()] = plugin;
                return plugin;
            }
        }
    }

    return nullptr;
}

std::vector<Plugin *> PluginManager::loadPlugins(const std::filesystem::path &directory)
{
    if (!exists(directory))
    {
        throw std::runtime_error("Provided directory does not exist.");
    }

    if (!is_directory(directory))
    {
        throw std::runtime_error("Provided path is not a directory.");
    }

    std::vector<Plugin *> loadedPlugins;

    // Traverse through the directory and its subdirectories.
    for (const auto &entry : std::filesystem::recursive_directory_iterator(directory))
    {
        if (std::filesystem::is_regular_file(entry.status()))
        {
            auto file = entry.path();

            try
            {
                auto plugin = loadPlugin(file);
                if (plugin)
                {
                    loadedPlugins.push_back(plugin);
                }
            }
            catch (std::exception &e)
            {
                server_.getLogger().error(
                    "Could not load '%s' in folder '%s': %s", file.c_str(), directory.c_str(), e.what());
            }
        }
    }

    return loadedPlugins;
}

void PluginManager::enablePlugin(const Plugin &plugin) const
{
    if (!plugin.isEnabled())
    {
        try
        {
            plugin.getPluginLoader()->enablePlugin(plugin);
        }
        catch (std::exception &e)
        {
            server_.getLogger().error("Error occurred (in the plugin loader) while enabling %s: %s",
                                      plugin.getDescription().getFullName().c_str(),
                                      e.what());
        }
    }
}

void PluginManager::disablePlugin(const Plugin &plugin) const
{
    if (plugin.isEnabled())
    {
        try
        {
            plugin.getPluginLoader()->disablePlugin(plugin);
        }
        catch (std::exception &e)
        {
            server_.getLogger().error("Error occurred (in the plugin loader) while disabling %s: %s",
                                      plugin.getDescription().getFullName().c_str(),
                                      e.what());
        }
    }
}

void PluginManager::disablePlugins()
{
    for (const auto &plugin : plugins_)
    {
        disablePlugin(*plugin);
    }
}

void PluginManager::clearPlugins()
{
    disablePlugins();
    plugins_.clear();
    lookupNames_.clear();
}
