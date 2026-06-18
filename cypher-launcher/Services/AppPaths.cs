using System;
using System.IO;

namespace Services
{
    public static class AppPaths
    {
        
        public static void Initialize()
        {
            InitializeFolders();
        }

        public static string ProjectsPath => Path.GetFullPath
        (
            Path.Combine(Path.GetDirectoryName(Environment.ProcessPath)!, "../../../../cypher-engine/Projects")
        );
        
        private static void InitializeFolders()
        {
            if (!Directory.Exists(ProjectsPath))
            {
                Directory.CreateDirectory(ProjectsPath); // creates all folders in path
            }
        }
    }
}