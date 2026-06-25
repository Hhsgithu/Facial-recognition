-- Create SeetaFace ID mapping table
-- This table maps SeetaFace internal IDs to database employeeIDs

CREATE TABLE IF NOT EXISTS seeta_face_mapping (
    seeta_face_id INTEGER PRIMARY KEY,
    employee_id INTEGER NOT NULL,
    create_time TEXT DEFAULT(datetime('now','localtime')),
    FOREIGN KEY (employee_id) REFERENCES employee(employeeID) ON DELETE CASCADE,
    UNIQUE(employee_id)
);

-- Create index for faster lookups
CREATE INDEX IF NOT EXISTS idx_seeta_employee ON seeta_face_mapping(employee_id);

-- Display table structure
SELECT 'Table created successfully' AS status;
SELECT * FROM seeta_face_mapping;
