#pragma once

#include "onnx/proto_utils.h"
#include "onnx/defs/data_type_utils.h"

namespace ONNX_NAMESPACE {

struct InferenceContext {
  virtual const AttributeProto* getAttribute(const std::string& name) const = 0;
  virtual size_t getNumInputs() const = 0;
  virtual const TypeProto_Tensor* getInputType(size_t index) const = 0;
  virtual size_t getNumOutputs() const = 0;
  virtual TypeProto_Tensor* getOutputType(size_t index) = 0;
  virtual ~InferenceContext() {}
};

typedef void (*InferenceFunction)(InferenceContext&);

template<typename T>
inline bool getRepeatedAttribute(InferenceContext& ctx,
                                 std::string attr_name,
                                 std::vector<T>& values) {
  const auto* attr = ctx.getAttribute(attr_name);
  if (attr) {
    values = RetrieveValues<T>(*attr);
    return true;
  } else {
    return false;
  }

}

inline bool hasExactlyNInputTypes(InferenceContext& ctx, int n, const std::string& opname) {
  if (static_cast<int>(ctx.getNumInputs()) != n) {
    throw std::runtime_error(opname + " has wrong number of inputs");
  }
  for (int i = 0; i < n; i++) {
    if (!ctx.getInputType(i)) {
      return false;
    }
  }
  return true;
}

inline void propagateElemTypeFromInputToOutput(InferenceContext& ctx, size_t inputIndex, size_t outputIndex) {
  ctx.getOutputType(outputIndex)->set_elem_type(ctx.getInputType(inputIndex)->elem_type());
}

inline void appendSingleDimCopiedFromInputTypeToOutputType(InferenceContext& ctx, size_t inputIndex, size_t outputIndex, size_t fromDimIndex) {
  auto* dim = ctx.getOutputType(outputIndex)->mutable_shape()->add_dim();
  *dim = ctx.getInputType(inputIndex)->shape().dim(static_cast<int>(fromDimIndex));
}

inline void propagateShapeFromInputToOutput(InferenceContext& ctx, size_t inputIndex, size_t outputIndex) {
  *ctx.getOutputType(outputIndex)->mutable_shape() = ctx.getInputType(inputIndex)->shape();
}

} // namespace ONNX_NAMESPACE
