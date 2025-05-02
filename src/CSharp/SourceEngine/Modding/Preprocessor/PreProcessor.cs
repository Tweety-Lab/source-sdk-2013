using SourceEngine.Entities;

namespace SourceEngine.Modding.PreProccesor
{
    /// <summary>
    /// PreProcessor for mods, handles things like registering ConVars from attributes and such.
    /// </summary>
    internal static class PreProcessor
    {
        // Process a mod assembly
        public static void Process(ModAssembly assembly)
        {
            ProcessConVars(assembly);
            ProcessEntities(assembly);
        }

        // Create ConVars for all properties that are marked with the [ConVar] attribute
        private static void ProcessConVars(ModAssembly assembly)
        {
            // Get all types in the assembly
            var types = assembly.Assembly.GetTypes();

            // Loop through all types
            foreach (var type in types)
            {
                // Get all properties in the type
                var properties = type.GetProperties();

                // Loop through all properties
                foreach (var property in properties)
                {
                    // Check if the property has the [ConVar] attribute
                    var conVarAttributes = property.GetCustomAttributes(typeof(ConVarAttribute), inherit: true);
                    foreach (var conVarAttribute in conVarAttributes)
                    {
                        if (conVarAttribute is ConVarAttribute attribute)
                        {
                            // Create a ConVar for the property using the property value as default value
                            var propValue = property.GetValue(null);
                            var propValueString = propValue?.ToString() ?? ""; // Handle null value gracefully

                            // Register the ConVar
                            Console.RegisterConVar(attribute.Name, propValueString, attribute.Flags, attribute.Description);
                        }
                    }
                }
            }
        }

        // Register Entities for all classes that are marked with the [Entity] attribute
        private static void ProcessEntities(ModAssembly assembly)
        {
            // Get all types in the assembly
            var types = assembly.Assembly.GetTypes();

            // Loop through all types
            foreach (var type in types)
            {
                // Check if the type has the [Entity] attribute
                var entityAttributes = type.GetCustomAttributes(typeof(EntityAttribute), inherit: true);

                foreach (var entityAttribute in entityAttributes)
                {
                    if (entityAttribute is EntityAttribute attribute)
                    {
                        // Register the entity
                        EntitySystem.RegisterEntity(attribute.Name, type);
                    }
                }
            }
        }
    }
}
