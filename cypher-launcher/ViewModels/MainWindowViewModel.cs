using System;
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
    public static ObservableCollection<ProjectItem> Projects {get; } = new();


    public static void LoadProjects()
    {
        Projects.Clear();
        foreach (var dir in Directory.GetDirectories(AppPaths.ProjectsPath))
            Projects.Add(new ProjectItem
            (
                Path.GetFileName(dir), 
                File.GetLastWriteTime(dir).ToString(), 
                TimeAgo(File.GetLastWriteTime(dir)),
                SizeMagnitude(GetFileSize(new DirectoryInfo(dir))),
                dir
            ));
    }

    private static string SizeMagnitude(long size)
    {
        long div = 1; 
        string label = "B";

        if (size >= (1000 * 1000 * 1000))
        {
            div = 1000 * 1000 * 1000;
            label = " GB";
        }
        else if (size >= (1000 * 1000))
        {
            div = 1000 * 1000;
            label = " MB";
        }
        else if (size >= 1000)
        {
            div = 1000;
            label = " KB";
        }
        return (size / div) + label;
    }
    private static string TimeAgo(DateTime dateTime)
    {
        string result;

        var timeSpan = DateTime.Now.Subtract(dateTime);

        if (timeSpan <= TimeSpan.FromSeconds(60))
        {
            result = string.Format("{0} seconds ago", timeSpan.Seconds);
        }
        else if (timeSpan <= TimeSpan.FromMinutes(60))
        {
            result = timeSpan.Minutes > 1 ?
                String.Format("{0} minutes ago", timeSpan.Minutes) :
                "a minute ago";
        }
        else if (timeSpan <= TimeSpan.FromHours(24))
        {
            result = timeSpan.Hours > 1 ?
                String.Format("{0} hours ago", timeSpan.Hours) :
                "an hour ago";
        }
        else if (timeSpan <= TimeSpan.FromDays(30))
        {
            result = timeSpan.Days > 1 ?
                String.Format("{0} days ago", timeSpan.Days) :
                "yesterday";
        }
        else if (timeSpan <= TimeSpan.FromDays(365))
        {
            result = timeSpan.Days > 30 ?
                String.Format("{0} months ago", timeSpan.Days / 30) :
                "a month ago";
        }
        else
        {
            result = timeSpan.Days > 365 ?
                String.Format("{0} years ago", timeSpan.Days / 365) :
                "a year ago";
        }

        return result;
    }

    private static long GetFileSize(DirectoryInfo directoryInfo)
    {
        long size = 0;

        FileInfo[] fis = directoryInfo.GetFiles();

        foreach (FileInfo fi in fis)
        {
            size += fi.Length;
        }

        DirectoryInfo[] dis = directoryInfo.GetDirectories();
        foreach (DirectoryInfo di in dis)
        {
            size += GetFileSize(di);
        }

        return size; 
    }

}

public record ProjectItem
(
    string Name, 
    string LastModified, 
    string TimeAgo,
    string Size,
    string Path
);