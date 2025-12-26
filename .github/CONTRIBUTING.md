# Pull Request Guidelines

Before submitting a Pull Request (PR), please carefully review these guidelines to ensure your contribution meets our standards!

## Code Guidelines

1. **Substantive Changes Only**
   - PRs should contain meaningful changes, not just syntax fixes or trivial modifications.
   - Example of what to avoid: [Pull Request #41](https://github.com/Lazap-Development/Lazap/pull/41)

2. **Working Condition**
   - Changes should be in a functional state before submission.
   - Ideally, the implementation should work perfectly with no issues.

3. **Practical Solutions**
   - Avoid temporary fixes for specific conditions.
   - Example of what to avoid: [Pull Request #32](https://github.com/Lazap-Development/Lazap/pull/32)

4. **Compatibility**
   - Ensure compatibility with current (non-deprecated) versions of:
     - CMake
     - MinGW (or MSVC)
     - Meson
     - Other external dependencies

5. **Versioning**
   - Follow [Semantic Versioning (SemVer)](https://semver.org) for Lazap version updates.

## PR Formatting Guidelines

1. **Title Requirements**
   - Start with a capital letter
   - Use proper capitalization (e.g., "Fix Login Authentication")
   - No grammatical errors
   - Edit immediately if mistakes are found

2. **Template Compliance**
   - Must follow the provided PR template
   - Non-compliant PRs will be closed

3. **Description Requirements**
   - Include all required information
   - Missing information may cause delays or closure

4. **Branch Selection**
   - Base branch should NOT be "main" by default
   - Exceptions require special approval
   - PRs with "main" as base will be closed immediately

## Best Practices

- Test your changes thoroughly before submission
- Include relevant test cases if applicable
- Document any significant changes in the PR description
- Reference related issues or discussions when applicable

By following these guidelines, you help maintain code quality and streamline the review process. Thank you!
