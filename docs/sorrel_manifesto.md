# SORREL’s True Purpose: A Constraint Engine for AI‑Human Collaboration

**SORREL is not merely a testing framework**. It is a **development structure** purpose‑built to make AI agents useful, disciplined collaborators for human engineers. 

> SORREL is **not** an operating system, an AI memory system, or an autonomous development framework. It **is** a discipline enforcer and a constraint engine around agent behavior.

## II. The Sip Principle: The Operational Heartbeat of SDD

**The Sip Principle**. A *sip* is a **single, minimal, verifiable, and disposable incremental action**—a development boundary that constrains how much an AI agent may attempt at one time. 

**Core Sip Rules:**
1. Implement one capability
2. Write minimal code
3. Provide compile instructions
4. Stop after the increment

### Three Agent Modes Within a Sip

| Mode | Activity |
|------|----------|
| **Understanding Mode** | Read the situation, review facts, cards, tools, and restrictions |
| **Planning Mode** | Determine the next minimal increment—which card, which tool, what expected output |
| **Coding Mode** | Perform the actual increment (write test, modify function), then **stop** |

## III. Numeric Evidence: The Critical Design Shift

> A Sorrel result is empirical evidence: a `key = number` measurement captured from the environment, **not a self‑declared pass/fail or true/false verdict**.

SDD removes self‑reporting mechanisms for correctness; agents must produce **numeric evidence instead of approval**.

## IV. The Restrictions Phase

SDD **inserts a Restrictions Phase before coding**:
`Requirements → Design → Restrictions → Coding → Maintenance`

The Restrictions phase defines structural constraints on **how code is allowed to be constructed**, not merely what it must achieve.

## V. Green Syntax: Machine‑First Structured Communication

| Element | Purpose |
|---------|---------|
| `TOOLS` | Defines available tools the agent may use |
| `PARAMETERS` | Defines situation inputs as structured data |
| `RESULTS` | Defines expected empirical measurements (must be numeric) |

## VI. The Reasoning Gate System (RGS)

| Gate | Function | Prevents |
|------|----------|----------|
| **Interpretation Gate** | Verifies understanding of objective | Solving the wrong problem |
| **Constraint Gate** | Ensures awareness of platform limits | Constraint drift |
| **Tool Selection Gate** | Validates tools exist in situation | Tool hallucination, misuse |
| **Strategy Gate** | Evaluates solution approach | Over‑engineering |
| **Scope Gate** | Checks modification scope | Uncontrolled expansion |
| **Navigation Gate** | Verifies codebase understanding | Incorrect file modification |

## VII. Human‑in‑the‑Loop as a First‑Class Component

| Human Responsibility | AI Responsibility |
|---------------------|-------------------|
| Define architectural intent | Language generation |
| Set environmental constraints | Structured reasoning |
| Declare tool availability | Problem decomposition |
| Design evaluation criteria | Implementation drafting |

## VIII. The SORREL Discovery Rule

> Agents must not assume capabilities; they must discover capabilities through cards. Only observations may produce facts.

## IX. The Dual‑Ledger System: Checkins and Checkouts

| Ledger | Purpose | Content |
|--------|---------|---------|
| `sorrel_checkins.md` | Planned, deferred, or unfinished sips | Intent preservation |
| `sorrel_checkouts.md` | Completed sips with evidence | Artifacts touched, numeric observations |

## X. Technical Implementation Details (card_runner)

1. **Prebuilt executables**: The runner discovers executables whose filenames match `.cpp` sources.
2. **Class-based dispatching**: Single C++ file contains multiple Logical Cards, dispatched via `argv[1]`.
3. **Situation-awareness**: Facts are grouped by `Situation:` headers.
4. **Decorators**: ` @Is`, ` @Needs`, ` @Results`, and ` @Situation` annotations control execution.
