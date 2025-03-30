setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Normal usage
# ------------

@test "kover quality runs correctly on an empty scene" {
  run kover quality < "$examples_dir"/empty.scene
  assert_success
  assert_output "Nothing to report, scene is empty"
}

# Wrong usage
# -----------

@test "kover quality reports an error when first line is invalid" {
  run kover quality < "$examples_dir"/first_line.invalid
  [ "$status" -eq 1 ]
  assert_output "error: first line must be exactly 'begin scene'"
}
