using System;

namespace SourceEngine
{

    /// <summary>
    /// Classes marked with this attribute will be registered as entities.
    /// </summary>
    [AttributeUsage(AttributeTargets.Class)]
    public class EntityAttribute : Attribute
    {
        public string Name { get; }
        public EntityAttribute(string name)
        {
            Name = name;
        }
    }
}
