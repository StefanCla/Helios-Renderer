#cmake_minimum_required(VERSION 3.23)

#add_library(DirectX)

#set_target_properties(DirectX PROPERTIES LINKER_LANGUAGE CXX)

# #target_sources (DirectX
#     PUBLIC
#         FILE_SET directXHeaders
#         TYPE HEADERS
#         BASE_DIRS
#             directx
#         FILES
#             directx/d3d12.h
#             directx/d3d12compatibility.h
#             directx/d3d12compiler.h
#             directx/D3D12Events.h
#             directx/d3d12sdklayers.h
#             directx/d3d12shader.h
#             directx/d3d12video.h
#             directx/d3dcommon.h
#             directx/d3dshadercacheregistration.h
#             directx/d3dx12.h
#             directx/d3dx12_barriers.h
#             directx/d3dx12_check_feature_support.h
#             directx/d3dx12_core.h
#             directx/d3dx12_default.h
#             directx/d3dx12_pipeline_state_stream.h
#             directx/d3dx12_property_format_table.h
#             directx/d3dx12_render_pass.h
#             directx/d3dx12_resource_helpers.h
#             directx/d3dx12_root_signature.h
#             directx/d3dx12_state_object.h
#             directx/DirectML.h
#             directx/dxcore.h
#             directx/dxcore_interface.h
#             directx/dxgicommon.h
#             directx/dxgiformat.h
#             directx/D3D12TokenizedProgramFormat.hpp
#     )