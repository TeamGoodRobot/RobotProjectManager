console.log("Toronto Robot Club script loaded!");

document.addEventListener('DOMContentLoaded', () => {
    const sectionHeadings = document.querySelectorAll('main section h2');

    sectionHeadings.forEach(heading => {
        const originalColor = heading.style.color; // This might be empty if not set inline
        heading.addEventListener('mouseover', () => {
            heading.style.color = '#007bff'; // A blue color for hover
        });
        heading.addEventListener('mouseout', () => {
            heading.style.color = originalColor || '#333'; // Revert to original or default
        });
    });
});
