project(
  'CS 32 Project 3',
  'cpp',
  version: '0.1.0',

  default_options: [
    'cpp_std=c++17',
    'warning_level=3',
    'werror=true',
    'buildtype=debugoptimized',
    'b_sanitize=address',
  ]
)

# Additional warning flags
add_project_arguments([
  '-Wno-sign-compare',
  '-Wno-unknown-pragmas',
  '-Werror=vla',
  '-Wfloat-equal',
  '-Wcast-align',
  '-Wstrict-overflow',
], language: 'cpp')

# source_files_to_zip = files(
# )

# zip = custom_target(
#   'zip',
#   output: 'cs32_hw1.zip',
#   input: source_files_to_zip,       # Mark these as input files
#   command: [
#     'zip',
#     '-j',
#     '-FS',
#     'cs32_hw1.zip',
#     '@INPUT@'                       # Meson expands this to all file paths
#   ],
#   depend_files: source_files_to_zip,
#   build_by_default: false
# )

subdir('src')
