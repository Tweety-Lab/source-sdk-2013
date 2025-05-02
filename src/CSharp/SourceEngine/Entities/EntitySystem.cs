using SourceEngine.Bridge.Entities;
using SourceEngine.Modding;
using System;
using System.Collections.Generic;
using System.Linq;

namespace SourceEngine.Entities
{

    /// <summary>
    /// Interaction with Entities.
    /// </summary>
    public static class EntitySystem
    {
        private static Dictionary<uint, BaseEntity> entities = new();
        private static uint nextId = 0;

        /// <summary>
        /// Creates an Entity from a namespace string.
        /// </summary>
        /// <returns>ID of the created Entity</returns>
        public static uint Create(string nameSpace)
        {
            // Attempt to find the type in loaded mod assemblies
            var type = Modding.ModAssembly.LoadedMods
                        .SelectMany(assembly => assembly.Assembly.GetTypes())
                        .FirstOrDefault(t => t.FullName == nameSpace)
                    ?? typeof(EntitySystem).Assembly.GetTypes()
                        .FirstOrDefault(t => t.FullName == nameSpace);

            // If no type found, return ID of 0
            // We might want to consider moving to int so we can return -1 since 0 could be a valid ID
            // Or just make IDs start at 1
            if (type == null)
            {
                Console.Warning($"Could not find Entity type {nameSpace}");
                return 0;
            }

            // Check if the type inherits from BaseEntity
            if (!typeof(BaseEntity).IsAssignableFrom(type))
            {
                Console.Warning($"Type {nameSpace} does not inherit from BaseEntity");
                return 0;
            }

            // Create and initialize the entity
            var entity = (BaseEntity)Activator.CreateInstance(type);
            uint id = ++nextId;
            entities[id] = entity;
            entity.SetNativeHandle(id);

            Console.Msg($"Creating Entity with id {id}");
            return id;
        }

        public static void Think(uint id)
        {
            if (entities.TryGetValue(id, out var entity))
                entity.Think();
        }

        public static void Spawn(uint id)
        {
            if (entities.TryGetValue(id, out var entity))
                entity.Spawn();
        }

        /// <summary>
        /// Registers an Entity name (i.e., math_counter) with an Entity Type (i.e., MathCounter).
        /// </summary>
        public static void RegisterEntity(string entityName, Type entityType)
        {
            // Convert System.Type to namespaced string (i.e., SourceEngine.Entities.MathCounter)
            string entityNamespace = entityType.FullName;

            // Convert string to UTF-8 byte array and pass to native code
            byte[] utf8Bytes = System.Text.Encoding.UTF8.GetBytes(entityNamespace);
            IntPtr namespacePtr = System.Runtime.InteropServices.Marshal.AllocHGlobal(utf8Bytes.Length + 1);
            System.Runtime.InteropServices.Marshal.Copy(utf8Bytes, 0, namespacePtr, utf8Bytes.Length);
            System.Runtime.InteropServices.Marshal.WriteByte(namespacePtr, utf8Bytes.Length, 0); // Null terminator

            EntityImports.CS_RegisterCSharpEntity(entityName, namespacePtr);
        }
    }

}