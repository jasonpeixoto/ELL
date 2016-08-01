////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     OutputPortElements.cpp (model)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "OutputPortElements.h"
#include "Node.h"

namespace model
{
    //
    // OutputPortElement
    //

    OutputPortElement::OutputPortElement(const OutputPortBase& port, size_t index) : _referencedPort(&port), _index(index) {}

    //
    // OutputPortRange
    //
    OutputPortRange::OutputPortRange(const OutputPortBase& port) : _referencedPort(&port), _startIndex(0), _numValues(port.Size()), _isFixedSize(false) {}

    OutputPortRange::OutputPortRange(const OutputPortBase& port, size_t index) : _referencedPort(&port), _startIndex(index), _numValues(1), _isFixedSize(true) {}

    OutputPortRange::OutputPortRange(const OutputPortBase& port, size_t startIndex, size_t numValues) : _referencedPort(&port), _startIndex(startIndex), _numValues(numValues), _isFixedSize(true) {}

    size_t OutputPortRange::Size() const
    {
        if (_isFixedSize)
        {
            return _numValues;
        }
        else
        {
            return _referencedPort->Size();
        }
    }

    size_t OutputPortRange::GetStartIndex() const { return _startIndex; }

    bool OutputPortRange::IsFullPortRange() const
    {
        return GetStartIndex() == 0 && Size() == ReferencedPort()->Size();
    }

    void OutputPortRange::Serialize(utilities::Serializer& serializer) const
    {
        serializer.Serialize("startIndex", _startIndex);
        serializer.Serialize("numValues", _numValues);
        serializer.Serialize("isFixedSize", _isFixedSize);
        serializer.Serialize("referencedNodeId", _referencedPort->GetNode()->GetId());
//        serializer.Serialize("referencedPortName", _referencedPort->GetName());
    }

    void OutputPortRange::Deserialize(utilities::Deserializer& serializer, utilities::SerializationContext& context)
    {
        throw "Not implemented";
    }

    //
    // OutputPortElementsUntyped
    //
    OutputPortElementsUntyped::OutputPortElementsUntyped(const OutputPortBase& port)
    {
        _ranges.emplace_back(port);
        ComputeSize();
    }

    OutputPortElementsUntyped::OutputPortElementsUntyped(const OutputPortBase& port, size_t startIndex)
    {
        _ranges.emplace_back(port, startIndex);
        ComputeSize();
    }

    OutputPortElementsUntyped::OutputPortElementsUntyped(const OutputPortBase& port, size_t startIndex, size_t numValues)
    {
        _ranges.emplace_back(port, startIndex, numValues);
        ComputeSize();
    }

    OutputPortElementsUntyped::OutputPortElementsUntyped(const OutputPortRange& range)
    {
        _ranges.push_back(range);
        ComputeSize();
    }

    OutputPortElementsUntyped::OutputPortElementsUntyped(const std::vector<OutputPortRange>& ranges)
    {
        _ranges.insert(_ranges.end(), ranges.begin(), ranges.end());
        ComputeSize();
    }
    
    void OutputPortElementsUntyped::AddRange(const OutputPortRange& range)
    {
        _ranges.push_back(range);
    }

    void OutputPortElementsUntyped::ComputeSize()
    {
        _size = 0;
        for (const auto& range : _ranges)
        {
            _size += range.Size();
        }
    }

    void OutputPortElementsUntyped::Serialize(utilities::Serializer& serializer) const
    {
        serializer.Serialize("ranges", _ranges);
    }

    void OutputPortElementsUntyped::Deserialize(utilities::Deserializer& serializer, utilities::SerializationContext& context)
    {
        throw "Not implemented";
    }
}
