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
  assert_output "Nothing to report, scene contains no construction"
}

@test "kover quality runs correctly on a scene with 1 building" {
  run kover quality < "$examples_dir"/1b.scene
  assert_success
  assert_output "building b1: E"
}

@test "kover quality runs correctly on a scene with 2 buildings" {
  run kover quality < "$examples_dir"/2b.scene
  assert_success
  assert_line --index 0 "building b1: E"
  assert_line --index 1 "building b2: E"
}

@test "kover quality runs correctly on a scene with 1 antenna" {
  run kover quality < "$examples_dir"/1a.scene
  assert_success
  assert_output "Nothing to report, scene contains no construction"
}

@test "kover quality runs correctly on a scene with 2 antennas" {
  run kover quality < "$examples_dir"/2a.scene
  assert_success
  assert_output "Nothing to report, scene contains no construction"
}

@test "kover quality runs correctly on a scene with 1 house" {
  run kover quality < "$examples_dir"/1h.scene
  assert_success
  assert_output "house h1: E"
}

@test "kover quality runs correctly on a scene with 2 houses" {
  run kover quality < "$examples_dir"/2h.scene
  assert_success
  assert_line --index 0 "house h1: E"
  assert_line --index 1 "house h2: E"
}

@test "kover quality runs correctly on a scene with 1 building and 1 antenna (0 corner)" {
  run kover quality < "$examples_dir"/1b1a_0corner.scene
  assert_success
  assert_output "building b: E"
}

@test "kover quality runs correctly on a scene with 1 building and 1 antenna (1 corner)" {
  run kover quality < "$examples_dir"/1b1a_1corner.scene
  assert_success
  assert_output "building b: D"
}

@test "kover quality runs correctly on a scene with 1 building and 1 antenna (2 corners)" {
  run kover quality < "$examples_dir"/1b1a_2corners.scene
  assert_success
  assert_output "building b: C"
}

@test "kover quality runs correctly on a scene with 1 building and 1 antenna (3 corners)" {
  run kover quality < "$examples_dir"/1b1a_3corners.scene
  assert_success
  assert_output "building b: B"
}

@test "kover quality runs correctly on a scene with 1 building and 1 antenna (4 corners)" {
  run kover quality < "$examples_dir"/1b1a_4corners.scene
  assert_success
  assert_output "building b: A"
}

@test "kover quality runs correctly on a scene with 1 building and 4 antennas (4 corners)" {
  run kover quality < "$examples_dir"/1b4a_4corners.scene
  assert_success
  assert_output "building b: A"
}

# Wrong usage
# -----------

@test "kover quality reports an error when first line is invalid" {
  run kover quality < "$examples_dir"/first_line.invalid
  [ "$status" -eq 1 ]
  assert_output "error: first line must be exactly 'begin scene'"
}
