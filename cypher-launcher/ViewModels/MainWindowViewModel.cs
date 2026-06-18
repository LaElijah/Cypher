using System.Collections.ObjectModel;
using System.IO;
using Avalonia.Interactivity;
using Services;

namespace cypher_launcher.ViewModels;

public partial class MainWindowViewModel : ViewModelBase
{
    public MainWindowViewModel()
    {
        LoadProjects();
    }
    public ObservableCollection<ProjectItem> Projects {get; } = new();


    public void LoadProjects()
    {
        Projects.Clear();
        foreach (var dir in Directory.GetDirectories(AppPaths.ProjectsPath))
            Projects.Add(new ProjectItem(Path.GetFileName(dir), dir));
    }

}

public record ProjectItem(string Name, string Path);