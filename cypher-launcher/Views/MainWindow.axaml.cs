using Avalonia.Controls;
using Avalonia.Interactivity;
using System;
using System.Diagnostics;
using System.IO;

namespace cypher_launcher.Views;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
    }

    private async void OnNewProjectClick(object? sender, RoutedEventArgs e)
    {
        var dialog = new CreateProjectWindow();
        var result = await dialog.ShowDialog<bool>(this); // 'this' = parent window

        if (result == true)
        {
            // User clicked Create
        }
    }

    private void Button_OnClick(object? sender, RoutedEventArgs e)
{
    string launcherDir = Path.GetDirectoryName(Environment.ProcessPath)!;

    string solutionRoot = Path.GetFullPath(Path.Combine(launcherDir, "../../../../"));
    string engineDir = Path.Combine(solutionRoot, "cypher-engine");
    string engineExeName = OperatingSystem.IsWindows() ? "Cypher.exe" : "Cypher";
    string enginePath = Path.Combine(engineDir, engineExeName);

    StatusLabel.Text = $"Looking for:\n{enginePath}";
    StatusLabel.Foreground = Avalonia.Media.Brushes.Gray;

    if (!File.Exists(enginePath))
    {
        StatusLabel.Text = $"Not found:\n{enginePath}";
        StatusLabel.Foreground = Avalonia.Media.Brushes.Red;
        return;
    }

    try
    {
        var startInfo = new ProcessStartInfo
        {
            FileName = enginePath,
            WorkingDirectory = engineDir, 
            UseShellExecute = true
        };

        Process? engineProcess = Process.Start(startInfo);
    }
    catch (Exception ex)
    {
        StatusLabel.Text = $"Launch failed:\n{ex.Message}";
        StatusLabel.Foreground = Avalonia.Media.Brushes.Red;
    }
}
}