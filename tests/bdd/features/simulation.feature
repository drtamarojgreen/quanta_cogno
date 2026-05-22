Feature: Genomic Simulation Accuracy
  As a researcher
  I want genomic simulations to be deterministic
  So that results are reproducible

  Scenario: SNP Impact on Gene Expression
    Given a gene "HTR2A" with expression 0.5
    And a SNP "rs6311" with impact 0.9
    When the simulation runs for 50 steps
    Then the expression of "HTR2A" should be greater than 0.5
