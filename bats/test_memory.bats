setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
  valgrind="valgrind --leak-check=full --error-exitcode=1"
}

@test "kover bounding-box handles memory correctly on a given scene" {
  $valgrind kover bounding-box < "$examples_dir"/3b2a.scene
}

@test "kover describe handles memory correctly on a given scene" {
  $valgrind kover describe < "$examples_dir"/3b2a.scene
}

@test "kover summarize handles memory correctly on a given scene" {
  $valgrind kover summarize < "$examples_dir"/3b2a.scene
}

@test "kover validate handles memory correctly on a given scene" {
  $valgrind kover validate < "$examples_dir"/3b2a.scene
}

@test "kover quality handles memory correctly on a given scene" {
  $valgrind kover quality < "$examples_dir"/3b2a.scene
}

@test "kover draw handles memory correctly on a given scene" {
  output_file="$BATS_TMPDIR/test_scene.svg"
  valgrind_with_suppressions="valgrind --leak-check=full --error-exitcode=1 --suppressions=$root_dir/cairo.supp"
  $valgrind_with_suppressions kover draw "$output_file" < "$examples_dir"/3b2a.scene
  [ -f "$output_file" ]
  rm -f "$output_file"
}
