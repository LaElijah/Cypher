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



        public static bool AddProject(string name)
        {
            string path = Path.Combine(ProjectsPath, name);

            if (Directory.Exists(path))
            {
                return false;
            }
            else
            {
                Directory.CreateDirectory(path);

                return true;
            }


        }
        
        private static void InitializeFolders()
        {
            if (!Directory.Exists(ProjectsPath))
            {
                Directory.CreateDirectory(ProjectsPath); // creates all folders in path
            }
        }
    }
}