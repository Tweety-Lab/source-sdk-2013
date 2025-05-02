using System;

namespace SourceEngine
{
    /// <summary>
    /// Static properties with this attribute will be registered as console variables.
    /// </summary>
    [AttributeUsage(AttributeTargets.Property)]
    public class ConVarAttribute : Attribute
    {
        public string Name { get; }

        public ConVarFlag Flags { get; }

        public string Description { get; }

        public ConVarAttribute(string name, ConVarFlag flags, string description)
        {
            Name = name;
            Flags = flags;
            Description = description;
        }
    }

}
