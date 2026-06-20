using Avalonia.Controls;
using Avalonia.Interactivity;
using Services;
using System;
using System.Diagnostics;
using System.IO;



namespace cypher_launcher.Views;

public partial class CreateProjectWindow : Window
{
    public CreateProjectWindow()
    {
        InitializeComponent();
    }

    private void OnCreateClick(object sender, RoutedEventArgs e)
    {
        // Return a result if using ShowDialog
        if (sender is Button button && button.Tag is string path)
            AppPaths.AddProject(path);

        Close(true);
    }

    private void OnCancelClick(object sender, RoutedEventArgs e)
    {
        Close(false);
    }
}